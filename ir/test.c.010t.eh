
;; Function main (main, funcdef_no=0, decl_uid=1830, cgraph_uid=0, symbol_order=0)

main ()
{
  int i;
  int D.1844;
  int D.1841;
  int D.1838;

  i = 0;
  goto <D.1835>;
  <D.1834>:
  D.1838 = i % 3;
  if (D.1838 == 0) goto <D.1839>; else goto <D.1840>;
  <D.1839>:
  D.1841 = i % 5;
  if (D.1841 == 0) goto <D.1842>; else goto <D.1843>;
  <D.1842>:
  printf ("%d is divisible by 3 and 5.\n", i);
  <D.1843>:
  <D.1840>:
  i = i + 1;
  <D.1835>:
  if (i <= 999) goto <D.1834>; else goto <D.1836>;
  <D.1836>:
  D.1844 = 0;
  goto <D.1845>;
  <D.1845>:
  return D.1844;
}


