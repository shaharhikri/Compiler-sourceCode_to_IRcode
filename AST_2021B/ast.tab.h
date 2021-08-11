/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_AST_TAB_H_INCLUDED
# define YY_YY_AST_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 17 "ast.y" /* yacc.c:1909  */

// #include "gen.h"
#include "ast.h"

#line 49 "ast.tab.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT_NUM = 258,
    FLOAT_NUM = 259,
    ADDOP = 260,
    MULOP = 261,
    RELOP = 262,
    ID = 263,
    READ = 264,
    IF = 265,
    ELSE = 266,
    WHILE = 267,
    FOR = 268,
    INT = 269,
    FLOAT = 270,
    OR = 271,
    AND = 272,
    NOT = 273,
    NAND = 274,
    SWITCH = 275,
    CASE = 276,
    DEFAULT = 277,
    BREAK = 278,
    CONTINUE = 279,
    XOR = 280
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 23 "ast.y" /* yacc.c:1909  */

   int ival;
   // float numbers in the source program are stored as double
   double fval;
   enum op op;
   char name[100];
   
   myType _type;
   
   //  pointers to AST nodes:
   Stmt *stmt;
   Block *block;
   Stmt *stmtlist; // points to first Stmt in the list (NULL if list is empty)
   ReadStmt *read_stmt;
   AssignStmt *assign_stmt;
   IfStmt *if_stmt;
   WhileStmt *while_stmt;
   SwitchStmt *switch_stmt;
   Case *caselist; //points to first Case in the list
   Case *mycase;
   BreakStmt *break_stmt;
   ContinueStmt *continue_stmt;
   Exp  *exp;
   BoolExp *boolexp;
   bool hasBreak;

#line 114 "ast.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_AST_TAB_H_INCLUDED  */
