#ifndef PASS1VISITOR_H_
#define PASS1VISITOR_H_

#include <iostream>
#include <string>

#include "wci/intermediate/SymTabStack.h"
#include "wci/intermediate/SymTabEntry.h"
#include "wci/intermediate/TypeSpec.h"

#include "ProjectBaseVisitor.h"
#include "antlr4-runtime.h"
#include "ProjectVisitor.h"

using namespace wci;
using namespace wci::intermediate;

class Pass1Visitor : public ProjectBaseVisitor
{
private:
    SymTabStack *symtab_stack;
    SymTabEntry *program_id;
    vector<SymTabEntry *> variable_id_list;
    ofstream j_file;

public:
    Pass1Visitor();
    virtual ~Pass1Visitor();

    ostream& get_assembly_file();

    bool lookupVariable(ProjectParser::VariableContext *ctx, SymTabEntry ** var_id);
    bool determineType(ProjectParser::TypeIDContext *ctx, TypeSpec ** type, string * type_indicator);

    antlrcpp::Any visitStringExpr(ProjectParser::StringExprContext *ctx) override;
    antlrcpp::Any visitNumber(ProjectParser::NumberContext *ctx) override;
    antlrcpp::Any visitSignedNumber(ProjectParser::SignedNumberContext *ctx) override;
    antlrcpp::Any visitIncrementExpr(ProjectParser::IncrementExprContext *ctx) override;
    antlrcpp::Any visitBoolExpr(ProjectParser::BoolExprContext *ctx) override;
    antlrcpp::Any visitVariableExpr(ProjectParser::VariableExprContext *ctx) override;
    antlrcpp::Any visitAddSubExpr(ProjectParser::AddSubExprContext *ctx) override;
    antlrcpp::Any visitExpr(ProjectParser::ExprContext *ctx) override;
    antlrcpp::Any visitProgram(ProjectParser::ProgramContext *ctx) override;
    antlrcpp::Any visitFunctionDefinition(ProjectParser::FunctionDefinitionContext *ctx) override;
    antlrcpp::Any visitFunctionID(ProjectParser::FunctionIDContext *ctx) override;
    antlrcpp::Any visitVariableID(ProjectParser::VariableIDContext *ctx) override;
    antlrcpp::Any visitVariable(ProjectParser::VariableContext *ctx) override;
    antlrcpp::Any visitStr(ProjectParser::StrContext *ctx) override;
    antlrcpp::Any visitMulDivModExpr(ProjectParser::MulDivModExprContext *ctx) override;
    antlrcpp::Any visitFuncCallExpr(ProjectParser::FuncCallExprContext *ctx) override;
    antlrcpp::Any visitUnsignedNumberExpr(ProjectParser::UnsignedNumberExprContext *ctx) override;
    antlrcpp::Any visitParenExpr(ProjectParser::ParenExprContext *ctx) override;
    antlrcpp::Any visitSignedNumberExpr(ProjectParser::SignedNumberExprContext *ctx) override;
    antlrcpp::Any visitPostinc(ProjectParser::PostincContext *ctx) override;
    antlrcpp::Any visitVariableDeclaration(ProjectParser::VariableDeclarationContext *ctx) override;
    antlrcpp::Any visitVariableDef(ProjectParser::VariableDefContext *ctx) override;
};

#endif /* PASS1VISITOR_H_ */
