#ifndef __TYPE_CONSTANT_HEADER__
#define __TYPE_CONSTANT_HEADER__

enum TypeTable {
#ifdef LISP_DEGRADE
	/* Debug */
	TypeTable_Debug1,
	TypeTable_Debug2,
	TypeTable_Debug3,
	TypeArgs_Debug1,
	TypeArgs_Debug2,
	TypeArgs_Debug3,
	TypeValues_Debug1,
	TypeValues_Debug2,
	TypeValues_Debug3,
	TypeCompiled_Debug1,
	TypeCompiled_Debug2,
	TypeCompiled_Debug3,
#endif

	/* Atomic-Type */
	TypeTable_Type,
	TypeTable_Asterisk,
	TypeTable_Atom,
	TypeTable_List,
	TypeTable_Boolean,
	TypeTable_Vector,
	TypeTable_SimpleVector,
	TypeTable_BitVector,
	TypeTable_ExtendedChar,
	TypeTable_SimpleBitVector,
	TypeTable_String,
	TypeTable_BaseString,
	TypeTable_SimpleString,
	TypeTable_SimpleBaseString,
	TypeTable_SignedByte,
	TypeTable_UnsignedByte,
	TypeTable_Bit,
	TypeTable_Fixnum,
	TypeTable_Bignum,
	TypeTable_Nil,
	TypeTable_T,
	TypeTable_Null,
	TypeTable_Cons,
	TypeTable_Hashtable,
	TypeTable_Symbol,
	TypeTable_Keyword,
	TypeTable_Package,
	TypeTable_RandomState,
	TypeTable_Readtable,
	TypeTable_Function,
	TypeTable_CompiledFunction,
	TypeTable_Pathname,
	TypeTable_LogicalPathname,
	TypeTable_Sequence,
	TypeTable_Array,
	TypeTable_SimpleArray,
	TypeTable_Character,
	TypeTable_BaseChar,
	TypeTable_StandardChar,
	TypeTable_Number,
	TypeTable_Real,
	TypeTable_Rational,
	TypeTable_Ratio,
	TypeTable_Integer,
	TypeTable_Complex,
	TypeTable_Float,
	TypeTable_ShortFloat,
	TypeTable_SingleFloat,
	TypeTable_DoubleFloat,
	TypeTable_LongFloat,
	TypeTable_Restart,
	TypeTable_Environment,
	TypeTable_Stream,
	TypeTable_BroadcastStream,
	TypeTable_ConcatenatedStream,
	TypeTable_EchoStream,
	TypeTable_FileStream,
	TypeTable_StringStream,
	TypeTable_SynonymStream,
	TypeTable_TwoWayStream,
	TypeTable_PromptStream,
	TypeTable_Quote,
	TypeTable_ByteSpec,

	/* Condition */
	TypeTable_ArithmeticError,
	TypeTable_FileError,
	TypeTable_PackageError,

	/* Type */
	TypeTable_Cxr,
	TypeTable_Cxar,
	TypeTable_Cxdr,
	TypeTable_Cxaar,
	TypeTable_Cxadr,
	TypeTable_Cxdar,
	TypeTable_Cxddr,
	TypeTable_Cxaaar,
	TypeTable_Cxaadr,
	TypeTable_Cxadar,
	TypeTable_Cxaddr,
	TypeTable_Cxdaar,
	TypeTable_Cxdadr,
	TypeTable_Cxddar,
	TypeTable_Cxdddr,
	TypeTable_SetfCxar,
	TypeTable_SetfCxdr,
	TypeTable_SetfCxaar,
	TypeTable_SetfCxadr,
	TypeTable_SetfCxdar,
	TypeTable_SetfCxddr,
	TypeTable_SetfCxaaar,
	TypeTable_SetfCxaadr,
	TypeTable_SetfCxadar,
	TypeTable_SetfCxaddr,
	TypeTable_SetfCxdaar,
	TypeTable_SetfCxdadr,
	TypeTable_SetfCxddar,
	TypeTable_SetfCxdddr,
	TypeTable_Fifth,
	TypeTable_Sixth,
	TypeTable_Seventh,
	TypeTable_Eighth,
	TypeTable_Ninth,
	TypeTable_Tenth,
	TypeTable_SetfFifth,
	TypeTable_SetfSixth,
	TypeTable_SetfSeventh,
	TypeTable_SetfEighth,
	TypeTable_SetfNinth,
	TypeTable_SetfTenth,
	TypeTable_CharacterNull,
	TypeTable_StringNull,
	TypeTable_StreamNull,
	TypeTable_Condition,
	TypeTable_ConditionNull,
	TypeTable_RestartNull,
	TypeTable_FunctionNull,
	TypeTable_EnvironmentNull,
	TypeTable_IntegerNull,
	TypeTable_PathnameNull,
	TypeTable_PackageNull,
	TypeTable_StringDesigner,
	TypeTable_PackageDesigner,
	TypeTable_FunctionDesigner,
	TypeTable_RestartDesigner,
	TypeTable_PathnameDesigner,
	TypeTable_StreamDesigner,
	TypeTable_ReadtableDesigner,
	TypeTable_Index,
	TypeTable_IndexNull,
	TypeTable_Intplus,
	TypeTable_IntplusNull,
	TypeTable_InputStream,
	TypeTable_OutputStream,
	TypeTable_TypeSpec,
	TypeTable_TypeSymbol,
	TypeTable_BitArray,
	TypeTable_SimpleBitArray,
	TypeTable_KeywordStart,
	TypeTable_KeywordEnd,
	TypeTable_KeyStart1End1,
	TypeTable_KeyStart2End2,
	TypeTable_FunctionName,
	TypeTable_RadixInteger,
	TypeTable_FloatSymbol,
	TypeTable_EqlT,
	TypeTable_CaseSensitivity,
	TypeTable_KeyTestList,
	TypeTable_RehashSize,
	TypeTable_RehashThreshold,
	TypeTable_CountKey,
	TypeTable_CountIfKey,
	TypeTable_PathnameHost,
	TypeTable_PathnameDevice,
	TypeTable_PathnameDirectory,
	TypeTable_PathnameName,
	TypeTable_PathnameType,
	TypeTable_PathnameVersion,
	TypeTable_Signed8,
	TypeTable_Signed16,
	TypeTable_Signed32,
	TypeTable_Unsigned8,
	TypeTable_Unsigned16,
	TypeTable_Unsigned32,
#ifdef LISP_64BIT
	TypeTable_Signed64,
	TypeTable_Unsigned64,
#endif
	TypeTable_OpenDirection,
	TypeTable_OpenElementType,
	TypeTable_OpenIfExists,
	TypeTable_OpenIfDoesNotExist,
	TypeTable_ExternalFormat,

	TypeTable_Method,
	TypeTable_Method1,
	TypeTable_Method2,
	TypeTable_Class,
	TypeTable_ClassNull,
	TypeTable_StandardClass,
	TypeTable_StandardObject,

	/* Array */
	TypeTable_Array_T,
	TypeTable_Array_Bit,
	TypeTable_Array_Character,
	TypeTable_Array_SingleFloat,
	TypeTable_Array_DoubleFloat,
	TypeTable_Array_LongFloat,
	TypeTable_Array_Signed8,
	TypeTable_Array_Signed16,
	TypeTable_Array_Signed32,
	TypeTable_Array_Unsigned8,
	TypeTable_Array_Unsigned16,
	TypeTable_Array_Unsigned32,
#ifdef LISP_64BIT
	TypeTable_Array_Signed64,
	TypeTable_Array_Unsigned64,
#endif

	/* Arguments */
	TypeArgs_OptConditionNull,
	TypeArgs_PackageDesigner,
	TypeArgs_PathnameCase,

	/* Values */
	TypeValues_Nil,
	TypeValues_T,
	TypeValues_Null,
	TypeValues_Cons,
	TypeValues_List,
	TypeValues_Boolean,
	TypeValues_Character,
	TypeValues_CharacterNull,
	TypeValues_Symbol,
	TypeValues_Vector,
	TypeValues_SimpleVector,
	TypeValues_String,
	TypeValues_StringNull,
	TypeValues_SimpleString,
	TypeValues_Stream,
	TypeValues_StreamNull,
	TypeValues_Function,
	TypeValues_EqlT,
	TypeValues_Package,
	TypeValues_PackageNull,
	TypeValues_Sequence,
	TypeValues_Array,
	TypeValues_Integer,
	TypeValues_Ratio,
	TypeValues_Rational,
	TypeValues_Index,
	TypeValues_IndexNull,
	TypeValues_Intplus,
	TypeValues_IntplusNull,
	TypeValues_Bit,
	TypeValues_BitArray,
	TypeValues_Pathname,
	TypeValues_PathnameNull,
	TypeValues_LogicalPathname,
	TypeValues_Float,
	TypeValues_Real,
	TypeValues_Number,
	TypeValues_Complex,
	TypeValues_TypeSymbol,
	TypeValues_Class,
	TypeValues_ClassNull,

	/* Compiled-Function */
	TypeCompiled_Object_Boolean,
	TypeCompiled_Symbol_Boolean,
	TypeCompiled_StringCase,
	TypeCompiled_NStringCase,
	TypeCompiled_StringTrim,
	TypeCompiled_StringEqual,
	TypeCompiled_StringMismatch,
	TypeCompiled_Rplaca,
	TypeCompiled_List_List,
	TypeCompiled_Nth,
	TypeCompiled_Nconc,
	TypeCompiled_Nreconc,
	TypeCompiled_ButLast,
	TypeCompiled_MacroFunction,
	TypeCompiled_MacroExpand,
	TypeCompiled_Abort,
	TypeCompiled_Continue,
	TypeCompiled_MacroReader,
	TypeCompiled_MacroDispatch,
	TypeCompiled_Read,
	TypeCompiled_Sublis,
	TypeCompiled_Subst,
	TypeCompiled_SubstIf,
	TypeCompiled_Eq,
	TypeCompiled_Every,
	TypeCompiled_Number_Equal,
	TypeCompiled_Number_Compare,
	TypeCompiled_Max,
	TypeCompiled_Minusp,
	TypeCompiled_Zerop,
	TypeCompiled_Plus,
	TypeCompiled_Minus,
	TypeCompiled_OnePlus,
	TypeCompiled_HashTableCount,
	TypeCompiled_Evenp,
	TypeCompiled_Export,
	TypeCompiled_UsePackage,
	TypeCompiled_Intern,
	TypeCompiled_PackageNicknames,
	TypeCompiled_Prin1,
	TypeCompiled_Prin1ToString,
	TypeCompiled_Reverse,
	TypeCompiled_Member,
	TypeCompiled_MemberIf,
	TypeCompiled_Mapc,
	TypeCompiled_Acons,
	TypeCompiled_Intersection,
	TypeCompiled_EcaseError,
	TypeCompiled_DoSymbols,
	TypeCompiled_ArrayBoolean,
	TypeCompiled_ArrayIndex,
	TypeCompiled_BitAnd,
	TypeCompiled_CountIf,
	TypeCompiled_Sort,
	TypeCompiled_FindIf,
	TypeCompiled_PositionIf,
	TypeCompiled_Search,
	TypeCompiled_Substitute,
	TypeCompiled_SubstituteIf,
	TypeCompiled_Remove,
	TypeCompiled_RemoveIf,
	TypeCompiled_RemoveDuplicates,
	TypeCompiled_Namestring,
	TypeCompiled_Pathname,
	TypeCompiled_InputStreamP,
	TypeCompiled_Exit,
	TypeCompiled_ReadChar,
	TypeCompiled_WriteString,
	TypeCompiled_FinishOutput,
	TypeCompiled_YesOrNoP,
	TypeCompiled_Floor,
	TypeCompiled_Ffloor,
	TypeCompiled_EnvInfo,
	TypeCompiled_Sin,
	TypeCompiled_RealPart,
	TypeCompiled_Gcd,
	TypeCompiled_Mod,
	TypeCompiled_FloatDigits,
	TypeCompiled_Rational,
	TypeCompiled_Logand,
	TypeCompiled_Logandc1,
	TypeCompiled_ByteSize,
	TypeCompiled_DepositField,
	TypeCompiled_Ldb,
	TypeCompiled_UpgradedType,
	TypeCompiled_SlotBoundp,
	TypeCompiled_SlotBoundp_Method,

	/* Size */
	TypeTable_Size
};

#endif
