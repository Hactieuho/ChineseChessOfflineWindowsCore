#pragma once
#include <jni.h>
//#include <android/log.h>
#include <string.h>
#include "pregen.h"
#include <stdio.h>
#include "../base/base2.h"
#include "../base/parse.h"
#include "ucci.h"
#include "position.h"
#include "hash.h"
#include "search.h"

#define DLL_EXPORT __declspec(dllexport)
//Chess engine interfaces
//void InitEngine();	//Always call this first

extern char g_bookPath[1024];
int Board[10][9];

//	bien toan cuc dung trong ham LetComputerThink
int srcX = 0;
int srcY = 0;
int dstX = 0;
int dstY = 0;
//Bien toan cuc dung cho ham isGameFinish
bool bCompWon = true;
//Bien toan cuc dung cho ham getAllLegalMove
int LegalCol[100];
int LegalRow[100];

//Set book path
extern "C" JNIEXPORT void JNICALL Java_com_mdc_core_ChessCore_SetBookPath(
		JNIEnv*, jclass, jstring);

extern "C" DLL_EXPORT void letComputerThink(int Board[10][9], int iDepth, long lngLimitTime,
		int* pSrcX, int* pSrcY, int* pDstX, int* pDstY);

extern "C" JNIEXPORT void JNICALL Java_com_mdc_core_ChessCore_LetComputerThink(
		JNIEnv *env, jclass cls, jobjectArray myArray, jint iDepth,
		jlong lngLimitTime, jobject src, jobject dst);

extern void letComputerThinkForHint(int Board[10][9], int iDepth, long lngLimitTime,
		int* pSrcX, int* pSrcY, int* pDstX, int* pDstY);
extern "C" JNIEXPORT void JNICALL Java_com_mdc_core_ChessCore_LetComputerThinkForHint(
		JNIEnv *, jclass, jobjectArray, jint, jlong, jobject src, jobject dst);

extern "C" DLL_EXPORT bool isLegalMove(int Board[10][9], int iSrcX, int iSrcY, int iDstX, int iDstY);
extern "C" JNIEXPORT jboolean JNICALL Java_com_mdc_core_ChessCore_IsLegalMove(
		JNIEnv *, jobject, jobjectArray, jint, jint, jint, jint);

extern bool isGameFinished(int Board[10][9], bool *bCompWon);
extern "C" JNIEXPORT jboolean JNICALL Java_com_mdc_core_ChessCore_IsGameFinished(
		JNIEnv*, jclass, jobjectArray, jobject chessBoard);

extern bool isBoardValid(int board[10][9], bool bCompFirst);
extern "C" JNIEXPORT jboolean JNICALL Java_com_mdc_core_ChessCore_IsBoardValid(
		JNIEnv*, jobject, jobjectArray, jboolean);
extern uint32_t genMoves(PositionStruct *lppos, uint32_t* lpmv);
// BAU 19/2/2009 v2.0
// hÃ m get cÃ¡c nÆ°á»›c Ä‘i há»£p lá»‡ cá»§a 1 quÃ¢n cá»� khi chÃ­ vÃ o nÃ³
int getAllLegalMoves(int Board[10][9], int iTestX, int iTestY, int *iLegalX,
		int *iLegalY);

extern "C" JNIEXPORT jint JNICALL Java_com_mdc_core_ChessCore_getAllLegalMoves(
		JNIEnv* env, jclass cls, jobjectArray myArray, jint TestX, jint TestY,
		jobject chessBoard);

extern bool quitGame();

extern "C" JNIEXPORT jboolean JNICALL Java_com_mdc_core_ChessCore_quitGame(
		JNIEnv*, jobject);

extern "C" JNIEXPORT void JNICALL Java_com_mdc_core_ChessCore_AvoidLoopMove(
		JNIEnv* env, jclass cls, jobject src, jobject dst);

extern "C" JNIEXPORT jboolean JNICALL Java_com_mdc_core_ChessCore_isCheck(
		JNIEnv* env, jobject obj, jobjectArray myArray, jint srcX, jint srcY,
		jint dstX, jint dstY);

#define LOG_TAG "chess-core"
#ifdef ANDROID
#  define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#  define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#else
#  define QUOTEME_(x) #x
#  define QUOTEME(x) QUOTEME_(x)
#  define LOGI(...) printf("I/" LOG_TAG " (" __FILE__ ":" QUOTEME(__LINE__) "): " __VA_ARGS__)
#  define LOGE(...) printf("E/" LOG_TAG "(" ")" __VA_ARGS__)
#endif

