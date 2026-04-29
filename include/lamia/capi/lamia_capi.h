/*
 * Lamia C API — public ABI for NGINX/Apache/MedusaServ-style module consumers.
 * © 2025 D Hargreaves AKA Roylepython. MIT licensed in this repository.
 */

#ifndef LAMIA_CAPI_H
#define LAMIA_CAPI_H

#ifdef __cplusplus
extern "C" {
#endif

/** Compile Lamia source to HTML. Caller frees returned string with lamia_free_string. */
const char* lamia_compile_to_html(const char* lamia_source);

/** Free string returned from lamia_compile_to_html. */
void lamia_free_string(const char* s);

/** Return 1 on success, 0 on failure (e.g. parse error). */
int lamia_compile_to_html_buf(const char* lamia_source, char* out_buf, unsigned out_size);

#ifdef __cplusplus
}
#endif

#endif /* LAMIA_CAPI_H */
