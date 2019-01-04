#include "com_mdc_core_ChessCore.h"
#include <jni.h>


//char g_bookPath[1024];

#ifdef _WIN32
#include <windows.h>
const char *const cszLibEvalFile = "EVALUATE.DLL";
const char *WINAPI GetEngineNameEVA(void);
int WINAPI EvaluateEVA(const PositionStruct *lppos, int vlAlpha, int vlBeta);
void WINAPI PreEvaluateEVA(PositionStruct *lppos, PreEvalStruct *lpPreEval);
#else
#include <dlfcn.h>
#define WINAPI
const char * const cszLibEvalFile = "libeval.so";
const char *WINAPI GetEngineNameEVA(void);
int WINAPI EvaluateEVA(const PositionStruct *lppos, int vlAlpha, int vlBeta);
void WINAPI PreEvaluateEVA(PositionStruct *lppos, PreEvalStruct *lpPreEval);

#endif

const int INTERRUPT_COUNT = 4096; // �������ɽ�������ж�

static const char *WINAPI GetEngineName(void) {
	return NULL;
}

static void WINAPI PreEvaluate(PositionStruct *lppos,
		PreEvalStruct *lpPreEval) {
	// ȱʡ�ľ���Ԥ���۹�̣�ʲô������
}

static int WINAPI Evaluate(const PositionStruct *lppos, int vlAlpha,
		int vlBeta) {
	// ȱʡ�ľ������۹�̣�ֻ����������ֵ
	return lppos->Material();
}

#ifdef _WIN32

inline HMODULE LoadEvalApi(const char *szLibEvalFile) {
	HMODULE hModule;
	hModule = LoadLibrary(szLibEvalFile);
	if (hModule == NULL) {
		Search.GetEngineName = GetEngineNameEVA;
		Search.PreEvaluate = PreEvaluateEVA;
		Search.Evaluate = EvaluateEVA;
	} else {
		Search.GetEngineName = (const char *(WINAPI *)(void)) GetProcAddress(hModule, "_GetEngineName@0");
		Search.PreEvaluate = (void (WINAPI *)(PositionStruct *, PreEvalStruct *)) GetProcAddress(hModule, "_PreEvaluate@8");
		Search.Evaluate = (int (WINAPI *)(const PositionStruct *, int, int)) GetProcAddress(hModule, "_Evaluate@12");
	}
	return hModule;
}

inline void FreeEvalApi(HMODULE hModule) {
	if (hModule != NULL) {
		FreeLibrary(hModule);
	}
}

#else

inline void *LoadEvalApi(const char *szLibEvalFile) {
	Search.GetEngineName = GetEngineNameEVA;
	Search.PreEvaluate = PreEvaluateEVA;
	Search.Evaluate = EvaluateEVA;
	return 0;
}

inline void FreeEvalApi(void *hModule) {
	if (hModule != NULL) {
		dlclose(hModule);
	}
}

#endif

/*
 * Set Book Path
 */JNIEXPORT void JNICALL Java_com_mdc_core_ChessCore_SetBookPath(JNIEnv* env,
		jclass cls, jstring bookPath) {
	const char *nativeString = env->GetStringUTFChars(bookPath, 0);
	strcpy(g_bookPath, nativeString);
	strcpy(Search.szBookFile, g_bookPath);
	LOGI("%s", g_bookPath);
	env->ReleaseStringUTFChars(bookPath, nativeString);
}

extern uint32_t g_dwMoveStr;
extern uint32_t g_dwSecondStr;
extern uint32_t g_dwThirdStr;

extern "C" JNIEXPORT void JNICALL Java_com_mdc_core_ChessCore_LetComputerThink(JNIEnv *env,
		jclass cls, jobjectArray myArray, jint iDepth, jlong lngLimitTime,
		jobject src, jobject dst) {
	//Load Board
	//int Board[10][9];
	for (int i = 0; i < 10; i++) {
		jintArray oneDim = (jintArray) env->GetObjectArrayElement(myArray, i);
		jint *element = env->GetIntArrayElements(oneDim, 0);
		for (int j = 0; j < 9; ++j) {
			Board[i][j] = element[j];
		}
		env->ReleaseIntArrayElements(oneDim, element, 0);
		env->DeleteLocalRef(oneDim);
	}

	int iSrcX = 0;
	int iSrcY = 0;
	int iDesX = 0;
	int iDesY = 0;

	letComputerThink(Board, iDepth, lngLimitTime, &iSrcX, &iSrcY, &iDesX,
			&iDesY);

	srcX = iSrcX;
	srcY = iSrcY;
	dstX = iDesX;
	dstY = iDesY;

	//	tim va load java class (ko tim thay, cls tra ve zero)
	cls = env->GetObjectClass(src);

	//	tim java method (ko tim thay, mid tra ve zero)
	if (cls != 0) {
		jmethodID mid = env->GetMethodID(cls, "set", "(II)V");

		//	goi java method
		if (mid != 0) {
			env->CallVoidMethod(src, mid, iSrcX, iSrcY);
			env->CallVoidMethod(dst, mid, iDesX, iDesY);
		} else {
			LOGI("Khong tim duoc method");
		}
	} else {
		LOGI("Khong tim duoc class : ChessCore");
		//return JNI_ERR;
	}
}

JNIEXPORT void JNICALL Java_com_mdc_core_ChessCore_LetComputerThinkForHint(
		JNIEnv *env, jclass cls, jobjectArray myArray, jint iDepth,
		jlong lngLimitTime, jobject src, jobject dst) {
	//Load Board
	for (int i = 0; i < 10; i++) {
		jintArray oneDim = (jintArray) env->GetObjectArrayElement(myArray, i);
		jint *element = env->GetIntArrayElements(oneDim, 0);
		for (int j = 0; j < 9; ++j) {
			Board[i][j] = element[j];
		}
		//Release Array, if not will overflow ref table
		env->ReleaseIntArrayElements(oneDim, element, 0);
		env->DeleteLocalRef(oneDim);
	}

	int iSrcX = 0;
	int iSrcY = 0;
	int iDesX = 0;
	int iDesY = 0;

	letComputerThinkForHint(Board, iDepth, lngLimitTime, &iSrcX, &iSrcY,
			&iDesX, &iDesY);

	srcX = iSrcX;
	srcY = iSrcY;
	dstX = iDesX;
	dstY = iDesY;

	//	tim va load java class (ko tim thay, cls tra ve zero)
	cls = env->GetObjectClass(src);

	//	tim java method (ko tim thay, mid tra ve zero)
	if (cls != 0) {
		jmethodID mid = env->GetMethodID(cls, "set", "(II)V");

		//	goi java method
		if (mid != 0) {
			env->CallVoidMethod(src, mid, iSrcX, iSrcY);
			env->CallVoidMethod(dst, mid, iDesX, iDesY);
		} else {
			LOGI("Khong tim duoc method");
		}
	} else {
		LOGI("Khong tim duoc class : ChessCore");
		//return JNI_ERR;
	}
}

extern "C" JNIEXPORT jboolean JNICALL Java_com_mdc_core_ChessCore_IsLegalMove(JNIEnv *env,
		jobject obj, jobjectArray myArray, jint srcCol, jint srcRow,
		jint dstCol, jint dstRow) {
//int localArray[10][9];
	for (int i = 0; i < 10; ++i) {
		jintArray oneDim = (jintArray) env->GetObjectArrayElement(myArray, i);
		jint *element = env->GetIntArrayElements(oneDim, 0);
		for (int j = 0; j < 9; ++j) {
			Board[i][j] = element[j];
		}
		//Release Array, if not will overflow ref table
		env->ReleaseIntArrayElements(oneDim, element, 0);
		env->DeleteLocalRef(oneDim);
	}
	return isLegalMove(Board, srcCol, srcRow, dstCol, dstRow);
}

//////////////////////////////////////////////////////////////////////////
//Check whether the game is finished,                                    //
//In this case the function returns TRUE and                            //
//                1. bCompWon = TRUE if the computer has won the game        //
//                2. bCompWon = FALSE if the player has won the game        //
//////////////////////////////////////////////////////////////////////////
//bool isGameFinished(int Board[10][9], bool* bCompWon) {
//	g_dwMoveStr = 0;
//	int i, j;
//	int pcWhite[7];
//	int pcBlack[7];
//	char fen[1024];
//	pcWhite[0] = SIDE_TAG(0) + KING_FROM;
//	pcWhite[1] = SIDE_TAG(0) + ADVISOR_FROM;
//	pcWhite[2] = SIDE_TAG(0) + BISHOP_FROM;
//	pcWhite[3] = SIDE_TAG(0) + ROOK_FROM;
//	pcWhite[4] = SIDE_TAG(0) + CANNON_FROM;
//	pcWhite[5] = SIDE_TAG(0) + KNIGHT_FROM;
//	pcWhite[6] = SIDE_TAG(0) + PAWN_FROM;
//
//	for (i = 0; i < 7; i++) {
//		pcBlack[i] = pcWhite[i] + 16;
//	};
//
////strcpy(Search.szBookFile, "BOOK.DAT");
////strcpy(Search.szBookFile, "/mnt/sdcard/BOOK.DAT");
//
//	LoadEvalApi("EVALUATE");
//	PreGenInit();
//	NewHash(24); // 24=16MB, 25=32MB, 26=64MB, ...
//	Search.pos.ClearBoard();
//
//	for (i = 0; i < 10; i++) {
//		for (j = 0; j < 9; j++) {
//			if ((Board[i][j] > 0) && (Board[i][j] < 8)) //Computer's pieces
//				Search.pos.AddPiece(COORD_XY(j + FILE_LEFT, 9 - i + RANK_TOP),
//						pcWhite[Board[i][j] - 1]);
//			else if (Board[i][j] > 0) //Human's pieces
//				Search.pos.AddPiece(COORD_XY(j + FILE_LEFT, 9 - i + RANK_TOP),
//						pcBlack[Board[i][j] - 8]);
//		};
//	};
//	memset(fen, 0, 1024);
//	Search.pos.ToFen(fen);
//	Search.pos.FromFen(fen);
//	Search.pos.nDistance = 0;
//	Search.PreEvaluate(&Search.pos, &PreEval);
//	Search.nBanMoves = 0;
//	Search.bIdle = Search.bPonder = Search.bQuit = Search.bBatch =
//			Search.bDebug = Search.bAlwaysCheck = false;
//	Search.bUseHash = true;
//	Search.bUseBook = Search.bNullMove = Search.bKnowledge = true;
//	Search.nCountMask = INTERRUPT_COUNT - 1;
//	Search.nRandomMask = 0;
//	Search.rc4Random.InitRand();
//	Search.nGoMode = GO_MODE_INFINITY;
//	SearchMain(1);
//	if (g_dwMoveStr == 0) {
//		*bCompWon = false;
//		DelHash();
//		return true;
//	} else {
//		Search.pos.ChangeSide();
//		SearchMain(1);
//		DelHash();
//		if (g_dwMoveStr == 0) {
//			*bCompWon = true;
//			return true;
//		};
//	};
//	return false;
//}

JNIEXPORT jboolean JNICALL Java_com_mdc_core_ChessCore_IsGameFinished(
		JNIEnv* env, jclass cls, jobjectArray myArray, jobject chessBoard) {
//int Board[10][9];
	for (int i = 0; i < 10; ++i) {
		jintArray oneDim = (jintArray) env->GetObjectArrayElement(myArray, i);
		jint *element = env->GetIntArrayElements(oneDim, 0);
		for (int j = 0; j < 9; ++j) {
			Board[i][j] = element[j];
		}
		env->ReleaseIntArrayElements(oneDim, element, 0);
		env->DeleteLocalRef(oneDim);
	}

	bool iBCompWon = true;
	bool ret = isGameFinished(Board, &iBCompWon);

	bCompWon = iBCompWon;

	//	tim va load java class (ko tim thay, cls tra ve zero)
	cls = env->GetObjectClass(chessBoard);

	//	tim java method (ko tim thay, mid tra ve zero)
	if (cls != 0) {
		jmethodID mid = env->GetMethodID(cls, "setRedWin", "(Z)V");

		//	goi java method
		if (mid != 0) {
			env->CallVoidMethod(chessBoard, mid, iBCompWon);
		} else {
			LOGI("Khong tim duoc method");
		}
	} else {
		LOGI("Khong tim duoc class : ChessCore");
		//return JNI_ERR;
	}
	return ret;
}


JNIEXPORT jboolean JNICALL Java_com_mdc_core_ChessCore_IsBoardValid(JNIEnv* env,
		jobject obj, jobjectArray myArray, jboolean bCompFirst) {
//int localArray[10][9];
	for (int i = 0; i < 10; ++i) {
		jintArray oneDim = (jintArray) env->GetObjectArrayElement(myArray, i);
		jint *element = env->GetIntArrayElements(oneDim, 0);
		for (int j = 0; j < 9; ++j) {
			Board[i][j] = element[j];
		}
		env->ReleaseIntArrayElements(oneDim, element, 0);
		env->DeleteLocalRef(oneDim);
	}
	return isBoardValid(Board, bCompFirst);
}

int getAllLegalMoves(int Board[10][9], int iTestCol, int iTestRow,
		jint *iLegalCol, jint *iLegalRow) {
	int i, j;
	int pcWhite[7];
	int pcBlack[7];
	char fen[1024];

	pcWhite[0] = SIDE_TAG(0) + KING_FROM;
	pcWhite[1] = SIDE_TAG(0) + ADVISOR_FROM;
	pcWhite[2] = SIDE_TAG(0) + BISHOP_FROM;
	pcWhite[3] = SIDE_TAG(0) + ROOK_FROM;
	pcWhite[4] = SIDE_TAG(0) + CANNON_FROM;
	pcWhite[5] = SIDE_TAG(0) + KNIGHT_FROM;
	pcWhite[6] = SIDE_TAG(0) + PAWN_FROM;

	for (i = 0; i < 7; i++) {
		pcBlack[i] = pcWhite[i] + 16;
	};

	SearchStruct localSearch;
	PreGenInit();
	localSearch.pos.ClearBoard();
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 9; j++) {
			if ((Board[i][j] > 0) && (Board[i][j] < 8)) //Computer's pieces
				localSearch.pos.AddPiece(
						COORD_XY(j + FILE_LEFT, 9 - i + RANK_TOP),
						pcWhite[Board[i][j] - 1]);
			else if (Board[i][j] > 0) //Human's pieces
				localSearch.pos.AddPiece(
						COORD_XY(j + FILE_LEFT, 9 - i + RANK_TOP),
						pcBlack[Board[i][j] - 8]);
		};
	};

	memset(fen, 0, 1024);
	localSearch.pos.ToFen(fen);
	localSearch.pos.FromFen(fen);
	localSearch.pos.nDistance = 0;
	localSearch.nBanMoves = 0;
	localSearch.bIdle = localSearch.bPonder = localSearch.bQuit =
			localSearch.bBatch = localSearch.bDebug = localSearch.bAlwaysCheck =
					false;
	localSearch.bUseHash = false;
	localSearch.bUseBook = localSearch.bNullMove = localSearch.bKnowledge =
			false;
	localSearch.nCountMask = INTERRUPT_COUNT - 1;
	localSearch.nRandomMask = 0;
	localSearch.rc4Random.InitRand();

	if (Board[iTestRow][iTestCol] > 7) {
		localSearch.pos.ChangeSide();
	}

	uint32_t lpmv[MAX_GEN_MOVES];
	uint32_t nCount = genMoves(&localSearch.pos, lpmv);

	uint32_t dwMoveStr;
	int nLegalCount = 0;
	for (int i = 0; i < nCount; i++) {
		dwMoveStr = MOVE_COORD(lpmv[i]);
		char* pMoveStr = (char*) (&dwMoveStr);
		if (pMoveStr != NULL && strlen(pMoveStr) > 0) {
			int iSrcCol = pMoveStr[0] - 'a';
			int iSrcRow = pMoveStr[1] - 48;
			int iDesCol = pMoveStr[2] - 'a';
			int iDesRow = pMoveStr[3] - 48;

			if (iTestCol == iSrcCol && iTestRow == iSrcRow) {
				iLegalCol[nLegalCount] = iDesCol;
				iLegalRow[nLegalCount] = iDesRow;
				nLegalCount++;
			}
		}
	}
	return nLegalCount;
}

JNIEXPORT jint JNICALL Java_com_mdc_core_ChessCore_getAllLegalMoves(JNIEnv* env,
		jclass cls, jobjectArray myArray, jint TestCol, jint TestRow,
		jobject chessBoard) {
//Load Board
//int Board[10][9];
	for (int i = 0; i < 10; ++i) {
		jintArray oneDim = (jintArray) env->GetObjectArrayElement(myArray, i);
		jint *element = env->GetIntArrayElements(oneDim, 0);
		for (int j = 0; j < 9; ++j) {
			Board[i][j] = element[j];
		}
		env->ReleaseIntArrayElements(oneDim, element, 0);
		env->DeleteLocalRef(oneDim);
	}

	jint test0[20];
	jint test1[20];
	jint *iLegalCol = &test0[0];
	jint *iLegalRow = &test1[0];

	int ret = getAllLegalMoves(Board, TestCol, TestRow, iLegalCol, iLegalRow);
	LOGI("Co %d nuoc di hop le", ret);

//Lay gia tri cot vao buffer int
	jintArray x_buffer;
	x_buffer = env->NewIntArray(ret);
	env->SetIntArrayRegion(x_buffer, 0, ret, iLegalCol);
//Lay gia tri hang vao buffer int
	jintArray y_buffer;
	y_buffer = env->NewIntArray(ret);
	env->SetIntArrayRegion(y_buffer, 0, ret, iLegalRow);

//thiet lap cac bien trong java
	JavaVMOption options[1]; //	cac setting cho jvm, chu y kich thuoc phai du lon de chua setting
	JavaVM *jvm; //	con tro tro toi JVM, dung de tao, khoi tao, huy JVM
	JavaVMInitArgs vm_args; // 	cac tham so de khoi tao JVM
	jint status; //	trang thai JVM (0 - thanh cong, JNI_ERR - khong thanh cong)
	jmethodID mid;

//	set classpath cho JVM
	strcpy(options[0].optionString, "-Djava.class.path=.");
	//options[0].optionString = "-Djava.class.path=.";

//	set aside memory cho vm_args (thiet lap xong thi moi khoi tao dc cac tham so nhu version, options...
	memset(&vm_args, 0, sizeof(vm_args));

//	thiet lap tham so khoi tao
	vm_args.version = JNI_VERSION_1_2; //	version java platform
	vm_args.nOptions = 1; //	giong kich thuoc cua options[1]
	vm_args.options = options;

//	tao JVM
	status = env->GetJavaVM(&jvm);

	if (status != JNI_ERR) {
		LOGI("Khoi tao JVM thanh cong");

		//	tim va load java class (ko tim thay, cls tra ve zero)
		cls = env->GetObjectClass(chessBoard);

		//	tim java method (ko tim thay, mid tra ve zero)
		if (cls != 0) {
			mid = env->GetMethodID(cls, "addLegalPoint", "([I[I)V");

			//	goi java method
			if (mid != 0) {
				env->CallVoidMethod(chessBoard, mid, x_buffer, y_buffer);
			} else {
				LOGI("Khong tim duoc method");
			}
		} else {
			LOGI("Khong tim duoc class");
			//return JNI_ERR;
		}
	}

	return ret;
}

//extern bool quitGame() {
//	return quit();
//}
JNIEXPORT jboolean JNICALL Java_com_mdc_core_ChessCore_quitGame(JNIEnv*,
		jobject) {
	return quit();
}

JNIEXPORT void JNICALL Java_com_mdc_core_ChessCore_AvoidLoopMove(JNIEnv* env,
		jclass cls, jobject src, jobject dst) {
	char* pSecondStr = (char*) (&g_dwSecondStr);
	int iSrcX = 0;
	int iSrcY = 0;
	int iDesX = 0;
	int iDesY = 0;
	if (pSecondStr != NULL && strlen(pSecondStr) > 0) {
		iSrcX = pSecondStr[0] - 'a';
		iSrcY = pSecondStr[1] - 48;
		iDesX = pSecondStr[2] - 'a';
		iDesY = pSecondStr[3] - 48;
	}
//	thiet lap cac bien trong java
	JavaVMOption options[1]; //	cac setting cho jvm, chu y kich thuoc phai du lon de chua setting
	JavaVM *jvm; //	con tro tro toi JVM, dung de tao, khoi tao, huy JVM
	JavaVMInitArgs vm_args; // 	cac tham so de khoi tao JVM
	jint status; //	trang thai JVM (0 - thanh cong, JNI_ERR - khong thanh cong)
	jmethodID mid;

//	set classpath cho JVM
	strcpy(options[0].optionString, "-Djava.class.path=.");
	//options[0].optionString = "-Djava.class.path=.";

//	set aside memory cho vm_args (thiet lap xong thi moi khoi tao dc cac tham so nhu version, options...
	memset(&vm_args, 0, sizeof(vm_args));

//	thiet lap tham so khoi tao
	vm_args.version = JNI_VERSION_1_2; //	version java platform
	vm_args.nOptions = 1; //	giong kich thuoc cua options[1]
	vm_args.options = options;

//	tao JVM
	status = env->GetJavaVM(&jvm);

	if (status != JNI_ERR) {
		LOGI("Khoi tao JVM thanh cong");

		//	tim va load java class (ko tim thay, cls tra ve zero)
		cls = env->GetObjectClass(src);

		//	tim java method (ko tim thay, mid tra ve zero)
		if (cls != 0) {
			mid = env->GetMethodID(cls, "set", "(II)V");

			//	goi java method
			if (mid != 0) {
				env->CallVoidMethod(src, mid, iSrcX, iSrcY);
				env->CallVoidMethod(dst, mid, iDesX, iDesY);
			} else {
				LOGI("Khong tim duoc method");
			}

		} else {
			LOGI("Khong tim duoc class : ChessCore");
			//return JNI_ERR;
		}
	}
}

/*
 * is Move Check
 */extern "C" JNIEXPORT jboolean JNICALL Java_com_mdc_core_ChessCore_isCheck(
		JNIEnv* env, jobject obj, jobjectArray myArray, jint src_x, jint src_y,
		jint dst_x, jint dst_y) {
	LOGI("JNI Move Check: %d %d %d %d", src_x, src_y, dst_x, dst_y);
	for (int i = 0; i < 10; i++) {
		jintArray oneDim = (jintArray) env->GetObjectArrayElement(myArray, i);
		jint *element = env->GetIntArrayElements(oneDim, 0);
		for (int j = 0; j < 9; ++j) {
			Board[i][j] = element[j];
			//LOGI("Board %d %d : %d", i, j, Board[i][j]);
		}
		env->ReleaseIntArrayElements(oneDim, element, 0);
		env->DeleteLocalRef(oneDim);
	}

	//is Red Turn ?
	bool isRedTurn = true;
	if (Board[src_y][src_x] > 7) {
		isRedTurn = false;
		LOGI("is Black Turn");
	} else {
		LOGI("is Red Turn");
	}
	//Find position of king piece
	int k_x = -1;
	int k_y = -1;
	if (isRedTurn) {
		//Find Black King
		for (int x = 3; x <= 5; x++)
			for (int y = 7; y <= 9; y++) {
				//LOGI("Black Board %d %d : %d", i, j, Board[i][j]);
				if (Board[y][x] == 8) {
					k_x = x;
					k_y = y;
					LOGI("Black King Position %d %d", x, y);
					break;
				}
			}
	} else {
		//Find Red King
		for (int x = 3; x <= 5; x++)
			for (int y = 0; y <= 3; y++) {
				//LOGI("Red Board %d %d : %d", i, j, Board[i][j]);
				if (Board[y][x] == 1) {
					k_x = x;
					k_y = y;
					LOGI("Red King Position %d %d", x, y);
					break;
				}
			}
	}
	if (k_x == -1 && k_y == -1) {
		LOGI("Error, cannot find King!");
		return false;
	}
	//Change board after move
	Board[dst_y][dst_x] = Board[src_y][src_x];
	Board[src_y][src_x] = 0;
	//is Check Move
	for (int x = 0; x < 9; x++)
		for (int y = 0; y < 10; y++) {
			if (Board[y][x] < 1)
				continue;
			else if ((Board[y][x] < 8 && isRedTurn)
					|| (Board[y][x] >= 8 && !isRedTurn)) {
				if (isLegalMove(Board, x, y, k_x, k_y)) {
					LOGI(
							"JNI Found Check Move: %d %d %d %d %d", Board[y][x], x, y, k_x, k_y);
					return true;
				}
			}
		}
	return false;
}
