#include "uip.h"
#include "psock.h"
#include "httpd.h"
#include "httpd_cgi.h"


/*---------------------------------------------------------------------------*/
static PT_THREAD(nullfunction(struct httpd_state *s, char *p))
{
  PSOCK_BEGIN(&s->sout);
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/

extern void log_Pcr(const char *str);

/*---------------------------------------------------------------------------*/
httpd_cgifunction httpd_cgi(char *name)
{
  log_Pcr(name);

  const struct httpd_cgi_call **f;

  /* Find the matching name in the table, return the function. */
  for(f = calls; *f != NULL; ++f) {
    if(strncmp_P((*f)->name, name, strlen((*f)->name)) == 0) {
      return (*f)->function;
    }
  }
  return nullfunction;
}
/*---------------------------------------------------------------------------*/
