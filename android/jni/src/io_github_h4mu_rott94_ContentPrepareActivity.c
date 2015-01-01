#include "io_github_h4mu_rott94_ContentPrepareActivity.h"

/*
 * Class:     io_github_h4mu_rott94_ContentPrepareActivity
 * Method:    isShareware
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_io_github_h4mu_rott94_ContentPrepareActivity_isShareware(JNIEnv * env, jobject self)
{
#if SHAREWARE
	return JNI_TRUE;
#else
	return JNI_FALSE;
#endif
}
