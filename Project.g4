grammar Project;

@header {
#include <stdlib.h>
#include "wci/intermediate/TypeSpec.h"
using namespace wci::intermediate;
extern string program_name;
}

program	: (stmt | functionDefinition)+
		;

declaration
	: typeID variableID					   # variableDeclaration
	;
	
definition  
	: typeID variableID ASSIGN expression    					  # variableDef
	;


functionDefinition  : typeID functionID '(' parameters? ')' 
					   '{'
					   		stmt?
					   		(RETURN expression ';')?
					   '}'
				     ;
 
functionCall : function '(' identifiers? ')' ;

identifiers  : expression  (',' expression)*  ;
parameters   : declaration (',' declaration)* ;

functionID   : IDENTIFIER ;
function     : IDENTIFIER ;
typeID       : IDENTIFIER ;

variableID   locals [ TypeSpec* type = nullptr ] : IDENTIFIER ;
variable     locals [ TypeSpec* type = nullptr, int size = 0 ] : IDENTIFIER ;
number       locals [ TypeSpec* type = nullptr ] : INTEGER    ;
str          locals [ TypeSpec* type = nullptr ] : STRING     ;
signedNumber locals [ TypeSpec* type = nullptr ] : sign number;

sign         : ADD_OP | SUB_OP ;

statement   : expressionStatement
			| printfStatement
			| forStatement
			| ifStatement
			| assignmentStatement
			| declarationStatement
			| definitionStatement
			| incrementStatement
			;
			
expressionStatement  : expression ';' ;
incrementStatement   : increment ';' ;
declarationStatement : declaration ';' ;
definitionStatement  : definition ';' ;

ifStatement	: IF '(' expr ')' '{' stmt? '}'
			(
				ELIF '(' expr ')' '{' stmt? '}'
			)*
			(
				ELSE '{' stmt? '}'
			)?
			;

forStatement : FOR '('
			 (
			 	(
			 		declaration ';' declaration ':' variable
			 	)
			 |
			 	(
					(declaration | definition | assignment) ';' expr ';' (assignment | expression)
			 	)
			 )
			')' '{' stmt? '}'
			 ;

printfStatement : PRINTF '(' str (',' identifiers)? ')' ';';

stmt       : statement (statement)* ;
assignmentStatement : assignment ';' ;
	
expression locals [ TypeSpec* type = nullptr ] 
	: functionCall                         # funcCallExpr
	| expression MUL_DIV_MOD_OP expression # mulDivModExpr
	| expression ADD_SUB_OP     expression # addSubExpr

	| BOOL                                 # boolExpr
	| str    							   # stringExpr
	| signedNumber                         # signedNumberExpr
	| increment							   # incrementExpr
	| number                               # unsignedNumberExpr
	| variable                             # variableExpr
	| '(' expression ')'				   # parenExpr	
	;

expr locals [ TypeSpec* type = nullptr ]
	: expression REL_COMP      expression
	;

increment locals [ TypeSpec* type = nullptr ]
		   : variable INC 								#postinc
		   ;

assignment : variable ASSIGN expression 			    # variableAssignment
		   ;

BOOL : TRUE | FALSE ;
// Operations (In Order of Precedence)
MUL_DIV_MOD_OP : MUL_OP | DIV_OP | MOD_OP ;
ADD_SUB_OP     : ADD_OP | SUB_OP          ;


// Comparators (In Order of Precedence)
REL_COMP  : EQ | NEQ | LT | LTE | GT | GTE ;

// Reserved Words
FOR        : 'for'    ;
IF         : 'if'     ;
ELSE       : 'else'   ;
ELIF       : 'elif'   ;
RETURN     : 'return' ;
TRUE       : 'true'   ;
FALSE      : 'false'  ;
PRINTF     : 'printf' ;


IDENTIFIER : [_a-zA-Z][_a-zA-Z0-9]* ;
INTEGER    : [0-9]+ ;

// Assign
ASSIGN : '=' ;

//Increment 
INC 	: '++';

// Mathematical Comparators
EQ     : '==' ;
NEQ    : '!=' ;
LT     : '<'  ;
LTE    : '<=' ;
GT     : '>'  ;
GTE    : '>=' ;

// Mathematical Operators
MUL_OP : '*' ;
DIV_OP : '/' ;
MOD_OP : '%' ;
ADD_OP : '+' ;
SUB_OP : '-' ;

STRING  : '"' .*? '"' ;
NEWLINE : '\r'? '\n' -> skip  ;		// Skip Return and Newlines
WS      : [ \t]+ -> skip ; 			// Skip Spaces and Tabs

