#include <stdio.h>
#include <stdlib.h> // exit ()
#include <stdarg.h>
#include <stack>

#include "ast.h"
#include "symtab.h"

/*    This stack is used to implement  continue statements.
      "continue"  is implemented as a goto to the label attached
      to the code for the condition of the most closely enclosing while
      statement. 
      This label is at the top of the stack. Below it on the stack is the 
      continue label of the enclosing while statement and so on.
      The stack is empty when we are not inside a  while statement.
*/

static	  
std::stack<int> continuelabels;

static
Object newTemp()
{
   static int counter = 1;
   char name[100];
   sprintf(name, "_t%d", counter++);
   return Object(name);
}

#if 0
static
int newTemp ()
{
   static int counter = 1;
   return counter++;
} 
#endif

// labels are represented by numbers. For example, 3 means label3
static
int newlabel ()
{
   static int counter = 1;
   return counter++;
} 

// emit works just like  printf  --  we use emit 
// to generate code and print it to the standard output.
void emit (const char *format, ...)
{
    // if (errors > 0) return; // do not generate code if there are errors.  This should be controlled by if !defined (DEBUG)) 

    printf ("    ");  // this is meant to add a nice indentation.
                      // Use emitlabel() to print a label without the indentation.    
    va_list argptr;
	va_start (argptr, format);
	// all the arguments following 'format' are passed on to vprintf
	vprintf (format, argptr); 
	va_end (argptr);
}

/* use this  to emit a label without using indentation */
void emitlabel (int label) 
{
    printf ("label%d:\n",  label);
}    

/* there are two versions of each arithmetic operator in the
   generated code. One is  used for  operands having type int. 
   The other is used for operands having type float.
*/   
struct operator_names { 
    const char *int_name; 
	const char *float_name; 
};

static
struct operator_names 
opNames [] = { {"+", "<+>"},
               {"-", "<->"},
			   {"*", "<*>"},
			   {"/", "</>"}};

/* convert operator  to  string  suitable for the given type
  e.g. opName (PLUS, _INT)  returns "+"
       opName (PLUS, _FLOAT) returns  "<+>"
*/
const char *
opName (enum op op, myType t)
{
    if (op > DIV) { fprintf (stderr, "internal compiler error #1"); exit (1); }
    if (t == _INT)
	    return opNames [op].int_name;
	else
	    return opNames [op].float_name;
}

//my addition
bool checkNumStr(char* s){
	int n = strlen(s);
	bool flag;
	char c;
	int i;
	for(i=0;i<n;i++){
		if(s[i] != '.'){
			flag = false;
			for(c='0';c<='9';c++){
				if(s[i]==c){
					flag = true;
					break;
				}
			}
			if(!flag)
				return false;
		}
	}
	return true;	
}
//end of my addition


Object BinaryOp::genExp ()
{
    //if (_left->_type != _right->_type)
    //    return Object(); //  this means an error was found
    	 	
	Object left_operand_result = _left->genExp ();
	Object right_operand_result = _right->genExp ();
	
	const char *the_op = opName (_op, _type);
	
	if (_left->_type != _right->_type){
		Object casting_var = newTemp();
		_right->_type = _left->_type;
		emit("%s = (%s) %s\n", casting_var._string, (_left->_type==_FLOAT ? "float" : "int"), right_operand_result._string);
		right_operand_result = casting_var;
	}
	else if(checkNumStr(left_operand_result._string)  &&  checkNumStr(right_operand_result._string)){
		NumNode* l = (NumNode*)(_left);
		NumNode* r = (NumNode*)(_right);
		Object num_obj;
		if(_left->_type==_INT){
			//printf("int----%s+%s=%s------\n",left_operand_result._string,right_operand_result._string,num_obj._string);
			int result = 0;
			int l_val = atoi(left_operand_result._string);
			int r_val = atoi(right_operand_result._string);
			switch (_op) {  //PLUS = 0, MINUS, MUL, DIV
				case PLUS:
					result = l_val+r_val;
					break;
				case MINUS:
					result = l_val-r_val;
					break;
				case MUL:
					result = l_val*r_val;
					break;
				default:
					result = l_val/r_val;
			}
			num_obj = Object(result);
		}
		else{
			//printf("flt----%s+%s=%s------\n",left_operand_result._string,right_operand_result._string,num_obj._string);
			float result = 0;
			float l_val = (float)atoi(left_operand_result._string);
			float r_val = (float)atoi(right_operand_result._string);
			switch (_op) {  //PLUS = 0, MINUS, MUL, DIV
				case PLUS:
					result = l_val+r_val;
					break;
				case MINUS:
					result = l_val-r_val;
					break;
				case MUL:
					result = l_val*r_val;
					break;
				default:
					result = l_val/r_val;
			}
			num_obj = Object(result);
		}
		
		return num_obj;
	}
	
	Object result = newTemp ();

  	emit ("%s = %s %s %s\n", result._string, left_operand_result._string, 
                                   the_op, right_operand_result._string);
	return result;
}

Object NumNode::genExp () 
{
    return (_type == _INT) ? Object(_u.ival) : Object(_u.fval);
#if 0
    int result = newTemp ();
	if (_type == _INT)
  	    emit ("_t%d = %d\n", result, _u.ival);
	else
	    emit ("_t%d = %.2f\n", result, _u.fval);
	return result;
#endif
}

Object IdNode::genExp()
{
    return Object(_name);
#if 0
    int result = newTemp ();
		
	emit ("_t%d = %s\n", result, _name);
	return result;
#endif
}

void SimpleBoolExp::genBoolExp (int truelabel, int falselabel)
{
    if (truelabel == FALL_THROUGH && falselabel == FALL_THROUGH)
	    return; // no need for code 

    const char *the_op;
	
	Object left_result = _left->genExp ();
	Object right_result = _right->genExp ();
	
    switch (_op) {
	    case LT:
		    the_op = "<";
			break;
		case GT:
		    the_op = ">";
			break;
		case LE:
		    the_op = "<=";
			break;
		case GE:
		    the_op = ">=";
			break;
		case EQ:
		    the_op = "==";
			break;
		case NE:
		    the_op = "!=";
			break;
		default:
		    fprintf (stderr, "internal compiler error #3\n"); exit (1);
	}
	
	if  (truelabel == FALL_THROUGH)
   	    emit ("ifFalse %s %s %s goto label%d\n", left_result._string, the_op,  //if bool false jump out
                  right_result._string, falselabel);
    else if (falselabel == FALL_THROUGH)
   	    emit ("if %s %s %s goto label%d\n", left_result._string, the_op,    	//if bool true jump out
                  right_result._string, truelabel);
	else { // no fall through
  	    emit ("if %s %s %s goto label%d\n", left_result._string, the_op,
                  right_result._string, truelabel);
	    emit ("goto label%d\n", falselabel);
	}
}

void Or::genBoolExp (int truelabel, int falselabel)
{
    if (truelabel == FALL_THROUGH && falselabel == FALL_THROUGH)
	    return; // no need for code 

    if  (truelabel == FALL_THROUGH) {
	    int next_label = newlabel(); // FALL_THROUGH implemented by jumping to next_label
	    _left->genBoolExp (next_label, // if left operand is true then the OR expression
		                               //is true so jump to next_label (thus falling through
									   // to the code following the code for the OR expression)
		                   FALL_THROUGH); // if left operand is false then 
						                  // fall through and evaluate right operand   
		_right->genBoolExp (FALL_THROUGH, falselabel);
        emitlabel (next_label);
    }  else if (falselabel == FALL_THROUGH) {
       _left->genBoolExp (truelabel, // if left operand is true then the OR expresson is true 
	                                 // so jump to  truelabel (without evaluating right operand)
                          FALL_THROUGH); // if left operand is false then 
						                  // fall through and evaluate right operand
	   _right->genBoolExp (truelabel, FALL_THROUGH);
	} else { // no fall through
	   _left->genBoolExp (truelabel, // if left operand is true then the or expresson is true 
	                                 // so jump to  truelabel (without evaluating right operand)
						  FALL_THROUGH); // if left operand is false then 
						                  // fall through and evaluate right operand
	   _right->genBoolExp (truelabel, falselabel);
	}
}

void And::genBoolExp (int truelabel, int falselabel)
{
    if (truelabel == FALL_THROUGH && falselabel == FALL_THROUGH)
	    return; // no need for code 
		
	if  (truelabel == FALL_THROUGH) {
	    _left->genBoolExp (FALL_THROUGH, // if left operand is true then fall through and evaluate
		                                 // right operand.
                           falselabel); // if left operand is false then the AND expression is
                                        // false so jump to falselabel);
        _right->genBoolExp (FALL_THROUGH, falselabel);
    } else if (falselabel == FALL_THROUGH) {
	    int next_label = newlabel(); // FALL_THROUGH implemented by jumping to next_label
        _left->genBoolExp (FALL_THROUGH, // if left operand is true then fall through and
                                         // evaluate right operand
                           next_label); // if left operand is false then the AND expression 
                                        //  is false so jump to next_label (thus falling through to
                                        // the code following the code for the AND expression)
        _right->genBoolExp (truelabel, FALL_THROUGH);
		emitlabel(next_label);
    } else { // no fall through
        _left->genBoolExp (FALL_THROUGH, 	// if left operand is true then fall through and
                                         // evaluate right operand
						   falselabel); // if left operand is false then the AND expression is false
						                // so jump to falselabel (without evaluating the right operand)
		_right->genBoolExp (truelabel, falselabel);
	}
}

void Nand::genBoolExp (int truelabel, int falselabel)
{
    if (truelabel == FALL_THROUGH && falselabel == FALL_THROUGH)
	    return; // no need for code 
		
	if  (truelabel == FALL_THROUGH) {
		int next_label = newlabel();
	    _left->genBoolExp (FALL_THROUGH,next_label);                                        
        _right->genBoolExp (falselabel, FALL_THROUGH);
		emitlabel (next_label);
    }
	else if (falselabel == FALL_THROUGH) {
	    _left->genBoolExp (FALL_THROUGH, falselabel);
        _right->genBoolExp (FALL_THROUGH, falselabel);
    } 
	else { 
        int next_label = newlabel();
	    _left->genBoolExp (FALL_THROUGH,next_label);                                        
        _right->genBoolExp (falselabel, truelabel);
		emitlabel (next_label);
	}
}

void Not::genBoolExp (int truelabel, int falselabel)
{
    _operand->genBoolExp (falselabel, truelabel); 
}

void ReadStmt::genStmt()
{
	myType idtype = _id->_type; 
	
	if (idtype == _INT)
 	  emit ("iread %s\n", _id->_name);
    else
      emit ("fread %s\n", _id->_name);
}

void AssignStmt::genStmt()
{
    Object result = _rhs->genExp();
	
	myType idtype = _lhs->_type; 
	
	 emit ("%s = %s%s\n", _lhs->_name,
					idtype == _rhs->_type ? "": (idtype==_FLOAT? "(float)":"(int)"),
										result._string);
}


void IfStmt::genStmt()
{
    int elseStmtlabel = newlabel ();
	int exitlabel = newlabel ();
	
	_condition->genBoolExp (FALL_THROUGH, elseStmtlabel);
	
    _thenStmt->genStmt ();
	emit ("goto label%d\n", exitlabel);
	emitlabel(elseStmtlabel);
    _elseStmt->genStmt();
	emitlabel(exitlabel);
}

void WhileStmt::genStmt()
{
    int condlabel = newlabel ();
	int exitlabel = newlabel ();
	
	emitlabel(condlabel);
	_condition->genBoolExp (FALL_THROUGH, exitlabel);
	
	
	
	_body->genStmt ();
	
	
	emit ("goto label%d\n", condlabel);
	emitlabel(exitlabel);
}

void Block::genStmt()
{
    for (Stmt *stmt = _stmtlist; stmt != NULL; stmt = stmt->_next)
	    stmt->genStmt();
}

void SwitchStmt::genStmt()
{ 
	Object switch_exp = _exp->genExp();
	int def_label,end_label;
	Case* c = _caselist;
	
	while (c != NULL){
			c->_label = newlabel();
			emit("if %s == %d goto label%d\n",switch_exp._string, c->_number,c->_label);
			c = c->_next;
	}
	def_label = newlabel();
	end_label = newlabel();
	emit("goto label%d\n",def_label);
	c = _caselist;
	while (c != NULL){
		emitlabel(c->_label);
		c->_stmt->genStmt();
		if(c->_hasBreak==true)
			emit("goto label%d\n",end_label);
		c = c->_next;
	}
	emitlabel(def_label);
	_default_stmt->genStmt();
	emitlabel(end_label);
}

void BreakStmt::genStmt()
{
    emit("break statements not implemented yet\n");
}
	
void ContinueStmt::genStmt()
{
    emit("continue statements not implemented yet\n");
}


