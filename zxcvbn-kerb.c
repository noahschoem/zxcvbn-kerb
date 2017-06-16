#include <krb5/krb5.h>
#include <krb5/plugin.h>
#include <krb5/pwqual_plugin.h>
#include <kadm5/admin.h>
#include <kadm5/kadm_err.h>
#include <zxcvbn.h>

static krb5_error_code check(krb5_context ctx,
			     krb5_pwqual_moddata data,
			     const char *password,
			     const char *policy_name,
			     krb5_principal princ,
			     const char **languages)
{
  /* Debian's zxcvbn-c claims that the dictionary is included in the object
     compiled from zxcvbn.c; hence we don't need to call ZxcvbnInit() or ZxxcvbnUnItit().
  */
  /* for now, null UserDict.  Later, should somehow contain the username. */
  /* for now, null Info too.  No plans to use this, unless we want to judge 
     based on something other than entropy at some point. */
  double entropy=ZxcvbnMatch(password,NULL,NULL);
  /* A StackExchange poster recommends this as a minimum. */
  if (entropy>=90.0)
    {
      return 0;
    }
  else
    {
      return KADM5_PASS_Q_GENERIC;
    }
}

krb5_error_code pwqual_zxcvbn_kerb_initvt(krb5_context context,
					  int maj_ver,
					  int min_ver,
					  krb5_plugin_vtable vtable)
{
  krb5_pwqual_vtable vt;
  
  if (maj_ver != 1)
    return KRB5_PLUGIN_VER_NOTSUPP;
  vt = (krb5_pwqual_vtable) vtable;
  vt->name = "zxcvbn_kerb";
  /*
    If I'm reading documentation correctly, we don't 
    need open and close; those are for opening a dictionary
    file (as per cracklib); we're passing control for that
    entirely off to zxcvbn-c.
  */
  //vt->open = init;
  vt->check = check;
  //vt->close = fini;
  return 0;
}
