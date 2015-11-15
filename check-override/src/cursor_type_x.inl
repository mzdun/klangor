#pragma once
#define CURSOR_KIND(X) \
	X(UnexposedDecl) \
	X(StructDecl) \
	X(UnionDecl) \
	X(ClassDecl) \
	X(EnumDecl) \
	X(FieldDecl) \
	X(EnumConstantDecl) \
	X(FunctionDecl) \
	X(VarDecl) \
	X(ParmDecl) \
	X(ObjCInterfaceDecl) \
	X(ObjCCategoryDecl) \
	X(ObjCProtocolDecl) \
	X(ObjCPropertyDecl) \
	X(ObjCIvarDecl) \
	X(ObjCInstanceMethodDecl) \
	X(ObjCClassMethodDecl) \
	X(ObjCImplementationDecl) \
	X(ObjCCategoryImplDecl) \
	X(TypedefDecl) \
	X(CXXMethod) \
	X(Namespace) \
	X(LinkageSpec) \
	X(Constructor) \
	X(Destructor) \
	X(ConversionFunction) \
	X(TemplateTypeParameter) \
	X(NonTypeTemplateParameter) \
	X(TemplateTemplateParameter) \
	X(FunctionTemplate) \
	X(ClassTemplate) \
	X(ClassTemplatePartialSpecialization) \
	X(NamespaceAlias) \
	X(UsingDirective) \
	X(UsingDeclaration) \
	X(TypeAliasDecl) \
	X(ObjCSynthesizeDecl) \
	X(ObjCDynamicDecl) \
	X(CXXAccessSpecifier) \
	X(ObjCSuperClassRef) \
	X(ObjCProtocolRef) \
	X(ObjCClassRef) \
	X(TypeRef) \
	X(CXXBaseSpecifier) \
	X(TemplateRef) \
	X(NamespaceRef) \
	X(MemberRef) \
	X(LabelRef) \
	X(OverloadedDeclRef) \
	X(VariableRef) \
	X(InvalidFile) \
	X(NoDeclFound) \
	X(NotImplemented) \
	X(InvalidCode) \
	X(UnexposedExpr) \
	X(DeclRefExpr) \
	X(MemberRefExpr) \
	X(CallExpr) \
	X(ObjCMessageExpr) \
	X(BlockExpr) \
	X(IntegerLiteral) \
	X(FloatingLiteral) \
	X(ImaginaryLiteral) \
	X(StringLiteral) \
	X(CharacterLiteral) \
	X(ParenExpr) \
	X(UnaryOperator) \
	X(ArraySubscriptExpr) \
	X(BinaryOperator) \
	X(CompoundAssignOperator) \
	X(ConditionalOperator) \
	X(CStyleCastExpr) \
	X(CompoundLiteralExpr) \
	X(InitListExpr) \
	X(AddrLabelExpr) \
	X(StmtExpr) \
	X(GenericSelectionExpr) \
	X(GNUNullExpr) \
	X(CXXStaticCastExpr) \
	X(CXXDynamicCastExpr) \
	X(CXXReinterpretCastExpr) \
	X(CXXConstCastExpr) \
	X(CXXFunctionalCastExpr) \
	X(CXXTypeidExpr) \
	X(CXXBoolLiteralExpr) \
	X(CXXNullPtrLiteralExpr) \
	X(CXXThisExpr) \
	X(CXXThrowExpr) \
	X(CXXNewExpr) \
	X(CXXDeleteExpr) \
	X(UnaryExpr) \
	X(ObjCStringLiteral) \
	X(ObjCEncodeExpr) \
	X(ObjCSelectorExpr) \
	X(ObjCProtocolExpr) \
	X(ObjCBridgedCastExpr) \
	X(PackExpansionExpr) \
	X(SizeOfPackExpr) \
	X(LambdaExpr) \
	X(ObjCBoolLiteralExpr) \
	X(ObjCSelfExpr) \
	X(UnexposedStmt) \
	X(LabelStmt) \
	X(CompoundStmt) \
	X(CaseStmt) \
	X(DefaultStmt) \
	X(IfStmt) \
	X(SwitchStmt) \
	X(WhileStmt) \
	X(DoStmt) \
	X(ForStmt) \
	X(GotoStmt) \
	X(IndirectGotoStmt) \
	X(ContinueStmt) \
	X(BreakStmt) \
	X(ReturnStmt) \
	X(GCCAsmStmt) \
	X(ObjCAtTryStmt) \
	X(ObjCAtCatchStmt) \
	X(ObjCAtFinallyStmt) \
	X(ObjCAtThrowStmt) \
	X(ObjCAtSynchronizedStmt) \
	X(ObjCAutoreleasePoolStmt) \
	X(ObjCForCollectionStmt) \
	X(CXXCatchStmt) \
	X(CXXTryStmt) \
	X(CXXForRangeStmt) \
	X(SEHTryStmt) \
	X(SEHExceptStmt) \
	X(SEHFinallyStmt) \
	X(MSAsmStmt) \
	X(NullStmt) \
	X(DeclStmt) \
	X(OMPParallelDirective) \
	X(OMPSimdDirective) \
	X(OMPForDirective) \
	X(OMPSectionsDirective) \
	X(OMPSectionDirective) \
	X(OMPSingleDirective) \
	X(OMPParallelForDirective) \
	X(OMPParallelSectionsDirective) \
	X(OMPTaskDirective) \
	X(OMPMasterDirective) \
	X(OMPCriticalDirective) \
	X(OMPTaskyieldDirective) \
	X(OMPBarrierDirective) \
	X(OMPTaskwaitDirective) \
	X(OMPFlushDirective) \
	X(SEHLeaveStmt) \
	X(OMPOrderedDirective) \
	X(OMPAtomicDirective) \
	X(OMPForSimdDirective) \
	X(OMPParallelForSimdDirective) \
	X(OMPTargetDirective) \
	X(OMPTeamsDirective) \
	X(OMPTaskgroupDirective) \
	X(OMPCancellationPointDirective) \
	X(OMPCancelDirective) \
	X(TranslationUnit) \
	X(UnexposedAttr) \
	X(IBActionAttr) \
	X(IBOutletAttr) \
	X(IBOutletCollectionAttr) \
	X(CXXFinalAttr) \
	X(CXXOverrideAttr) \
	X(AnnotateAttr) \
	X(AsmLabelAttr) \
	X(PackedAttr) \
	X(PureAttr) \
	X(ConstAttr) \
	X(NoDuplicateAttr) \
	X(CUDAConstantAttr) \
	X(CUDADeviceAttr) \
	X(CUDAGlobalAttr) \
	X(CUDAHostAttr) \
	X(CUDASharedAttr) \
	X(PreprocessingDirective) \
	X(MacroDefinition) \
	X(MacroExpansion) \
	X(InclusionDirective) \
	X(ModuleImportDecl) \
	X(OverloadCandidate)
