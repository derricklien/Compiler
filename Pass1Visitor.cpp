#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>


#include "Pass1Visitor.h"
#include "wci/intermediate/SymTabFactory.h"
#include "wci/intermediate/symtabimpl/Predefined.h"
#include "wci/util/CrossReferencer.h"

using namespace std;
using namespace wci;
using namespace wci::intermediate;
using namespace wci::intermediate::symtabimpl;
using namespace wci::util;

extern string program_name;
extern unordered_map<string, int> sizeTable;
static string function_name = "";

static SymTabEntry *variable_id;

#include <stdio.h>
static string EXCEPTION(string message)
{
	{
		string filename = program_name + ".j";
		char file[filename.length() + 1];
		strcpy(file, filename.c_str());

		std::remove(file);
	}
	exit(1);
}


bool Pass1Visitor::determineType(ProjectParser::TypeIDContext *ctx, TypeSpec ** type, string * type_indicator)
{
	if(ctx == NULL)
	{
		EXCEPTION("No type found");
	}

	string type_name = ctx->IDENTIFIER()->toString();
    
    bool success = true;
    if(type_name == "int")
    {
        *type = Predefined::integer_type;
        *type_indicator = "I";
    }
    else if (type_name == "bool")
    {
        *type = Predefined::boolean_type;
        *type_indicator = "Z";
    }
    else if (type_name == "string")
    {
        *type = Predefined::char_type;
        *type_indicator = "Ljava/lang/String;";
    }
    else if (type_name == "void")
    {
        *type = Predefined::void_type;
        *type_indicator = "V";
    }
    else
    {
        *type = nullptr;
        *type_indicator = "?";
        success = false;
    }
    return success;
}

bool Pass1Visitor::lookupVariable(ProjectParser::VariableContext *ctx, SymTabEntry ** var_id)
{
	if(ctx == NULL)
	{
		EXCEPTION("Variable is missing");
	}

	string variable_name = function_name + ctx->IDENTIFIER()->toString();
    *var_id = symtab_stack->lookup(variable_name);

    return *var_id != NULL;
}

Pass1Visitor::Pass1Visitor()
    : program_id(nullptr), j_file(nullptr)
{
    // Create and initialize the symbol table stack.
    symtab_stack = SymTabFactory::create_symtab_stack();
    Predefined::initialize(symtab_stack);

//    cout << "=== Pass1Visitor(): symtab stack initialized." << endl;
}

Pass1Visitor::~Pass1Visitor() {}

ostream& Pass1Visitor::get_assembly_file() { return j_file; }

antlrcpp::Any Pass1Visitor::visitProgram(ProjectParser::ProgramContext *ctx)
{   
    // Visit Header Block
    // cout << "=== visitProgram: " + ctx->getText() << endl;

    program_id = symtab_stack->enter_local(program_name);
    program_id->set_definition((Definition)DF_PROGRAM);
    program_id->set_attribute((SymTabKey) ROUTINE_SYMTAB,
                              symtab_stack->push());
    symtab_stack->set_program_id(program_id);

    // Create the assembly output file.
    j_file.open(program_name + ".j");
    if (j_file.fail())
    {
            cout << "***** Cannot open assembly file." << endl;
            exit(-99);
    }

    // Emit the program header.
    j_file << ".class public " << program_name << endl;
    j_file << ".super java/lang/Object" << endl;

    // Emit the RunTimer and PascalTextIn fields.
    j_file << endl;
    j_file << ".field private static _runTimer LRunTimer;" << endl;
    j_file << ".field private static _standardIn LPascalTextIn;" << endl;

    auto value = visitChildren(ctx);

    // Emit the class constructor.
    j_file << endl;
    j_file << ".method public <init>()V" << endl;
    j_file << endl;
    j_file << "\taload_0" << endl;
    j_file << "\tinvokenonvirtual    java/lang/Object/<init>()V" << endl;
    j_file << "\treturn" << endl;
    j_file << endl;
    j_file << ".limit locals 1" << endl;
    j_file << ".limit stack 1" << endl;
    j_file << ".end method" << endl;

       // cout << "=== visitProgram: Printing xref table." << endl;

//    // Print the cross-reference table.
//    CrossReferencer cross_referencer;
//    cross_referencer.print(symtab_stack);

    return value;
}

antlrcpp::Any Pass1Visitor::visitVariableDeclaration(ProjectParser::VariableDeclarationContext *ctx)
{
    // cout << "=== visitVariableDeclarations: " << ctx->getText() << endl;
    
    variable_id = nullptr;
    
    auto value = visitChildren(ctx);

    TypeSpec *type;
    string type_indicator;

    if(determineType(ctx->typeID(), &type, &type_indicator) == false)
    {
        EXCEPTION("Invalid type!");
    }

    // set the variable type
    variable_id->set_typespec(type);

    // Emit a field declaration.
    j_file << ".field private static " << variable_id->get_name() << " " << type_indicator << endl;
    
    return value;
}

antlrcpp::Any Pass1Visitor::visitVariableDef(ProjectParser::VariableDefContext *ctx)
{
    // cout << "=== visitVariableDefs: " << ctx->getText() << endl;
    variable_id = nullptr;

    auto value = visitChildren(ctx);

    TypeSpec *type;
    string type_indicator;

    if(determineType(ctx->typeID(), &type, &type_indicator) == false)
    {
        EXCEPTION("Invalid type!");
    }

    // Set the Variable Type
    variable_id->set_typespec(type);

    // Emit a field declaration.
    j_file << ".field private static " << variable_id->get_name() << " " << type_indicator << endl;

    return value;
}

antlrcpp::Any Pass1Visitor::visitFunctionDefinition(ProjectParser::FunctionDefinitionContext *ctx)
{
    // cout << "=== visitFunctionDefinition: " + ctx->getText() << endl;

    function_name = ctx->functionID()->getText() + "_";
        
    variable_id = nullptr;
    auto value = visit(ctx->functionID());
    visit(ctx->typeID());

    TypeSpec *type;
    string type_indicator;
    
    if(determineType(ctx->typeID(), &type, &type_indicator) == false)
    {
        EXCEPTION("Invalid type!");
    }
    
    variable_id->set_typespec(type);

    visit(ctx->parameters());
    visit(ctx->stmt());
    visit(ctx->expression());
    function_name = "";

    return value;
}

antlrcpp::Any Pass1Visitor::visitFunctionID(ProjectParser::FunctionIDContext *ctx)
{
    // cout << "=== visitFunctionID: " + ctx->getText() << endl;

    string func_name = ctx->IDENTIFIER()->toString();
    
    if(symtab_stack->lookup(func_name) != NULL)
    {
        EXCEPTION("Redeclaration of function");
    }

    variable_id = symtab_stack->enter_local(func_name);
    variable_id->set_definition((Definition) DF_FUNCTION);

    return visitChildren(ctx);
}

antlrcpp::Any Pass1Visitor::visitVariableID(ProjectParser::VariableIDContext *ctx)
{
    // cout << "=== visitVariableID: " + ctx->getText() << endl;

    string variable_name = function_name + ctx->IDENTIFIER()->toString();
    
    if(symtab_stack->lookup(variable_name) != NULL)
    {
        EXCEPTION("Redeclaration of variable");
    }

    variable_id = symtab_stack->enter_local(variable_name);
    variable_id->set_definition((Definition) DF_VARIABLE);

    return visitChildren(ctx);
}

 antlrcpp::Any Pass1Visitor::visitVariable(ProjectParser::VariableContext *ctx)
 {
     // cout << "=== visitVariable: " + ctx->getText() << endl;

     if(lookupVariable(ctx, &variable_id) == false)
     {
         EXCEPTION("Variable is not defined");
     }

	 ctx->type = variable_id->get_typespec();
     return visitChildren(ctx);
 }

antlrcpp::Any Pass1Visitor::visitVariableExpr(ProjectParser::VariableExprContext *ctx)
{
    // cout << "=== visitVariableExpr: " + ctx->getText() << endl;
    auto value =  visitChildren(ctx);
    ctx->type = ctx->variable()->type;

    return value;
}

antlrcpp::Any Pass1Visitor::visitAddSubExpr(ProjectParser::AddSubExprContext *ctx)
{
   // cout << "=== visitAddSubExpr: " + ctx->getText() << endl;

    auto value = visitChildren(ctx);
    
    TypeSpec *type1 = ctx->expression(0)->type;
    TypeSpec *type2 = ctx->expression(1)->type;

    bool integer_mode =    (type1 == Predefined::integer_type)
                        && (type2 == Predefined::integer_type);

    TypeSpec *type = integer_mode ? Predefined::integer_type
                   :                nullptr;
    ctx->type = type;

    return value;
}

antlrcpp::Any Pass1Visitor::visitNumber(ProjectParser::NumberContext *ctx)
{
    // cout << "=== visitNumber: " + ctx->getText() << endl;

    ctx->type = Predefined::integer_type;
    
    return visitChildren(ctx);
}

antlrcpp::Any Pass1Visitor::visitSignedNumber(ProjectParser::SignedNumberContext *ctx)
{
    // cout << "=== visitSignedNumber: " + ctx->getText() << endl;

    auto value = visit(ctx->number());
    ctx->type = ctx->number()->type;

    return value;
}

antlrcpp::Any Pass1Visitor::visitSignedNumberExpr(ProjectParser::SignedNumberExprContext *ctx)
{
   // cout << "=== visitSignedNumberExpr: " + ctx->getText() << endl;

    auto value = visitChildren(ctx);
    ctx->type = ctx->signedNumber()->type;

    return value;
}

antlrcpp::Any Pass1Visitor::visitBoolExpr(ProjectParser::BoolExprContext *ctx)
{
   // cout << "=== visitBoolExpr: " + ctx->getText() << endl;

    auto value = visitChildren(ctx);
    ctx->type = Predefined::boolean_type;

    return value;
}

antlrcpp::Any Pass1Visitor::visitStr(ProjectParser::StrContext *ctx)
{
	 // cout << "=== visitStrExpr: " + ctx->getText() << endl;
	 
     ctx->type = Predefined::char_type;

	 return visitChildren(ctx);
}

antlrcpp::Any Pass1Visitor::visitStringExpr(ProjectParser::StringExprContext *ctx)
{
	   // cout << "=== visitStringExpr: " + ctx->getText() << endl;
	    
        auto value = visitChildren(ctx);
	    ctx->type = ctx->str()->type;

	    return value;
}

antlrcpp::Any Pass1Visitor::visitUnsignedNumberExpr(ProjectParser::UnsignedNumberExprContext *ctx)
{
    // cout << "=== visitUnsignedNumberExpr: " + ctx->getText() << endl;
    
    auto value = visitChildren(ctx);
    ctx->type = ctx->number()->type;

    return value;
}

antlrcpp::Any Pass1Visitor::visitExpr(ProjectParser::ExprContext *ctx)
{
    // cout << "=== visitExpr: " + ctx->getText() << endl;

    auto value = visitChildren(ctx);
    ctx->type = Predefined::boolean_type;

    return value;
}

antlrcpp::Any Pass1Visitor::visitMulDivModExpr(ProjectParser::MulDivModExprContext *ctx)
{
    // cout << "=== visitMulDivExpr: " + ctx->getText() << endl;
    auto value = visitChildren(ctx);

    TypeSpec *type1 = ctx->expression(0)->type;
    TypeSpec *type2 = ctx->expression(1)->type;

    bool integer_mode =    (type1 == Predefined::integer_type)
                        && (type2 == Predefined::integer_type);

    TypeSpec *type = integer_mode ? Predefined::integer_type
                   :                nullptr;
    ctx->type = type;

    return value;
}

antlrcpp::Any Pass1Visitor::visitFuncCallExpr(ProjectParser::FuncCallExprContext *ctx)
{
    // cout << "=== visitFuncCallExpr: " + ctx->getText() << endl;

    string func_name = ctx->functionCall()->function()->IDENTIFIER()->toString();
    SymTabEntry *function_id = symtab_stack->lookup(func_name);

    if(function_id == NULL)
    {
    	EXCEPTION("Function is not defined");
    }
    ctx->type = function_id->get_typespec();

    return visitChildren(ctx);
}

antlrcpp::Any Pass1Visitor::visitParenExpr(ProjectParser::ParenExprContext *ctx)
{
    // cout << "=== visitParenExpr: " + ctx->getText() << endl;

    auto value = visitChildren(ctx);
    ctx->type = ctx->expression()->type;

    return value;
}

antlrcpp::Any Pass1Visitor::visitIncrementExpr(ProjectParser::IncrementExprContext *ctx)
{
    // cout << "=== visitIncrementExpr: " + ctx->getText() << endl;

    auto value = visitChildren(ctx);
    ctx->type = ctx->increment()->type;

    return value;
}

 antlrcpp::Any Pass1Visitor::visitPostinc(ProjectParser::PostincContext *ctx)
 {
	// cout << "=== visitPostInc: " + ctx->getText() << endl;
	
    auto value = visitChildren(ctx);
    
    if(lookupVariable(ctx->variable(), &variable_id) == false)
    {
        EXCEPTION("Variable is not defined");
    }
	ctx->type = variable_id->get_typespec();

	return value;
 }
