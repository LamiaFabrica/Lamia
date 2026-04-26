#if defined(_WIN32)
#define LAMIA_TEST_EXPORT extern "C" __declspec(dllexport)
#else
#define LAMIA_TEST_EXPORT extern "C" __attribute__((visibility("default")))
#endif

LAMIA_TEST_EXPORT int lamia_sample_add(int lhs, int rhs)
{
    const int result = lhs + rhs;
    return result;
}

LAMIA_TEST_EXPORT const char* lamia_sample_identity()
{
    static const char identity[] = "lamia-runtime-shared-library-test-plugin";
    const char* result = identity;
    return result;
}
