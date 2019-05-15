#ifndef PASS2VISITOR_H_
#define PASS2VISITOR_H_

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

class Pass2Visitor : public ProjectBaseVisitor
{
private:
	ostream& j_file;

public:
	Pass2Visitor(ostream& j_file);
    virtual ~Pass2Visitor();

    antlrcpp::Any visitProgram(ProjectParser::ProgramContext *ctx) override;
    antlrcpp::Any visitBoolExpr(ProjectParser::BoolExprContext *ctx) override;
    antlrcpp::Any visitStr(ProjectParser::StrContext *ctx) override;
    antlrcpp::Any visitFunctionDefinition(ProjectParser::FunctionDefinitionContext *ctx) override;
    antlrcpp::Any visitFunctionCall(ProjectParser::FunctionCallContext *ctx) override;
    antlrcpp::Any visitNumber(ProjectParser::NumberContext *ctx) override;
    antlrcpp::Any visitSignedNumber(ProjectParser::SignedNumberContext *ctx) override;
    antlrcpp::Any visitIfStatement(ProjectParser::IfStatementContext *ctx) override;
    antlrcpp::Any visitForStatement(ProjectParser::ForStatementContext *ctx) override;
    antlrcpp::Any visitVariableExpr(ProjectParser::VariableExprContext *ctx) override;
    antlrcpp::Any visitAddSubExpr(ProjectParser::AddSubExprContext *ctx) override;
    antlrcpp::Any visitExpr(ProjectParser::ExprContext *ctx) override;
    antlrcpp::Any visitIncrementExpr(ProjectParser::IncrementExprContext *ctx) override;
    antlrcpp::Any visitMulDivModExpr(ProjectParser::MulDivModExprContext *ctx) override;
    antlrcpp::Any visitPostinc(ProjectParser::PostincContext *ctx) override;
    antlrcpp::Any visitPrintfStatement(ProjectParser::PrintfStatementContext *ctx) override;
    antlrcpp::Any visitVariableDef(ProjectParser::VariableDefContext *ctx) override;
    antlrcpp::Any visitVariableAssignment(ProjectParser::VariableAssignmentContext *ctx) override;
};

#endif /* PASS2VISITOR_H_ */
