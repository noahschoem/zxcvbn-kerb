zxcvbn-kerb: Kerberos integration layer for zxcvbn

This thing is **most definitely not** production ready.  Please make no 
attempt to use it.

If you want to try testing it, it is recommended to do so on Debian Sid, or Jessie with backports, and have Kerberos 5 and the zxcvbn-c package installed.

To build, compile with "gcc -c zxcvbn-kerb.c -o zxcvbn-kerb.so".

To use, edit your krb5.conf to include the following lines:

[plugins]
  pwqual = {
    module = strength:@moduledir@/zxcvbn-kerb.so
  }