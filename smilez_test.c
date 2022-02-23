#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "smilez.h"


/* 1/2 of these are aromatic, 1/2 of these are Kekule */
const char *smiles_list[] = {
  "c1cc(ccc1CCNC(=O)CNc2ccc(cc2)C#N)O", 
  "CC(=O)N1CCN(CC1)Cc2ccc(cc2)C(=O)NCCc3ccc(cc3)O", 
  "CC(C(=O)NCCc1ccc(cc1)O)Oc2ccc(cc2[N+](=O)[O-])Cl", 
  "c1ccc(cc1)n2cc(c(n2)c3ccc(cc3)F)CC(=O)NCCc4ccc(cc4)O", 
  "COc1c(cc(cc1F)NC(=O)CNc2ccc(cc2)C#N)F", 
  "CC(C(=O)Nc1cc([nH]n1)CC2CCCCC2)OCC3CC3", 
  "CC(C(=O)N1CCc2c(cccc2NC(=O)C)C1)Oc3ccc(cc3[N+](=O)[O-])Cl", 
  "c1cc(c2c(c1)OCCO2)NC(=O)CNc3ccc(cc3)C#N", 
  "c1ccc(c(c1)C(=O)N)OCc2cccc(c2)C(=O)Nc3cccc4c3OCCO4", 
  "CC(C(=O)Nc1cc(c(c(c1)F)N(C)C)F)OCC2CC2", 
  "CC(C(=O)NCC(F)(F)F)NC(=O)CNc1ccc(cc1)C#N", 
  "c1cc(ccc1C#N)NCC(=O)NCCC(=O)NC2CCCCC2", 
  "CC(C(=O)NCCC(=O)NC1CCCCC1)Oc2ccc(cc2[N+](=O)[O-])Cl", 
  "CC(c1ccc(cc1)NC(=O)c2ccc(cc2)F)NC(=O)C(C)OCC3CC3", 
  "CC(C(=O)Nc1ccc(cc1)Oc2cccc(c2)OC)OCC3CC3", 
  "COc1ccccc1CN2CCC(CC2)NC(=O)CNc3ccc(cc3)C#N", 
  "CN(C)CC(Cc1ccccc1)NC(=O)CNc2ccc(cc2)C#N", 
  "CC(=O)N1CCN(CC1)Cc2ccc(cc2)C(=O)Nc3cc(c(c(c3)F)N(C)C)F", 
  "Cc1csc(n1)NC(=O)C2CCN(CC2)C(=O)CNc3ccc(cc3)C#N", 
  "Cc1csc(n1)NC(=O)C2CCN(CC2)C(=O)C(C)OCC3CC3", 
  "Cc1csc(n1)NC(=O)C2CCCN(C2)C(=O)C(C)OCC(C)C", 
  "CNC(=O)C(c1ccccc1)NC(=O)CNc2ccc(cc2)C#N", 
  "CC(C(=O)NC(c1ccccc1)C(=O)NC)Oc2ccc(cc2[N+](=O)[O-])Cl", 
  "Cc1ccccc1OCCNC(=O)c2ccc(cc2)CN3CCN(CC3)C(=O)C", 
  "Cc1ccc2nc(cn2c1)CNC(=O)c3ccc(cc3)CN4CCN(CC4)C(=O)C", 
  "Cc1c2c(cc(nc2n(n1)c3ccccn3)C4CC4)C(=O)NC(C)C(=O)NC", 
  "CCC(c1ccc2c(c1)OCCO2)NC(=O)CNc3ccc(cc3)C#N", 
  "CCC(c1cccs1)NC(=O)c2ccc(cc2)CN3CCN(CC3)C(=O)C", 
  "CC(C)N(C)Cc1ccc(cc1)CNC(=O)CNc2ccc(cc2)C#N", 
  "c1cc(ccc1C#N)NCC(=O)N2CCC(CC2)C(=O)c3ccc(c(c3)Cl)Cl", 
  "CN(CCOC)C(=O)Cc1cn(nc1c2ccc(cc2)F)c3ccccc3", 
  "CNC(=O)C(c1ccccc1)NC(=O)c2cccc(c2)COc3ccccc3C(=O)N", 
  "Cc1ccccc1OCCNC(=O)CNc2ccc(cc2)C#N", 
  "CC(C)Oc1ccccc1CNC(=O)c2ccc(cc2)CN3CCN(CC3)C(=O)C", 
  "CCCN(CC(=O)NC)C(=O)c1cc(nc2c1c(nn2c3ccccn3)C)C4CC4", 
  "CCCN(CC(=O)NC)C(=O)c1cnc(s1)c2ccc(s2)Cl", 
  "Cc1ccc2nc(cn2c1)CNC(=O)c3cccc(c3)COc4ccccc4C(=O)N", 
  "CC(C(=O)NC)NC(=O)c1cccc(c1)COc2ccccc2C(=O)N", 
  "CCCN(CC(=O)NC)C(=O)Cc1cn(nc1c2ccc(cc2)F)c3ccccc3", 
  "Cc1nc(on1)C2CCCN(C2)C(=O)c3ccc(cc3)CN4CCN(CC4)C(=O)C", 
  "Cc1nc(on1)C2CCN(CC2)C(=O)c3ccc(cc3)CN4CCN(CC4)C(=O)C", 
  "CN(CCCNC(=O)CNc1ccc(cc1)C#N)c2ccccc2F", 
  "c1cc(cc(c1)NC(=O)CNc2ccc(cc2)C#N)c3cn4c(n3)CCCC4", 
  "CC(C(=O)Nc1ccc(cc1)c2cn3c(n2)CCCC3)OCC4CC4", 
  "Cc1c(oc(n1)c2ccc(cc2)NC(=O)C(C)Oc3ccc(cc3[N+](=O)[O-])Cl)C", 
  "CC(C(=O)N(C)CCCc1cc(n[nH]1)c2cccc(c2)F)OCC3CC3", 
  "CC(C)(C)c1nnc(s1)NC(=O)c2cnc(s2)c3ccc(s3)Cl", 
  "Cc1nc(on1)C2CCN(CC2)C(=O)c3cnc(s3)c4ccc(s4)Cl", 
  "Cc1c(oc(n1)c2ccc(cc2)NC(=O)CNc3ccc(cc3)C#N)C", 
  "CC(C)COC(C)C(=O)N(C)CCCc1cc(n[nH]1)c2cccc(c2)F", 
  "CC(=O)N1CCN(CC1)Cc2ccc(cc2)C(=O)Nc3nnc(s3)C(C)(C)C", 
  "c1cc(cc(c1)NC(=O)c2ccncc2)CNC(=O)CNc3ccc(cc3)C#N", 
  "CCCOc1ccc(c(c1)C)NC(=O)c2ccc(cc2)CN3CCN(CC3)C(=O)C", 
  "CC(C(=O)NCc1cccc(c1)NC(=O)Cn2cccn2)OCC3CC3", 
  "c1cc(cc(c1)Cn2cccn2)CNC(=O)CNc3ccc(cc3)C#N", 
  "c1cc(cc(c1)NC(=O)Cn2cccn2)CNC(=O)CNc3ccc(cc3)C#N", 
  "Cc1cccc(c1)C2CCCN2C(=O)CNc3ccc(cc3)C#N", 
  "CCCc1c(sc(n1)NC(=O)c2ccc(cc2)CN3CCN(CC3)C(=O)C)CC", 
  "CC(=O)N1CCN(CC1)Cc2ccc(cc2)C(=O)N3CCC(CC3)C(=O)NC4CC4", 
  "c1cc(sc1c2ncc(s2)C(=O)N3CCC(CC3)C(=O)NC4CC4)Cl", 
  "CN1C2CCC1CC(C2)CNC(=O)CNc3ccc(cc3)C#N", 
  "CN1C2CCC1CC(C2)CNC(=O)c3cccc(c3)COc4ccccc4C(=O)N", 
  "CC(C(=O)NCCCS(=O)(=O)N)Oc1ccc(cc1[N+](=O)[O-])Cl", 
  "c1cc(sc1c2ncc(s2)C(=O)NCCCS(=O)(=O)N)Cl", 
  "Cc1c(sc(n1)C)C(C)NC(=O)c2ccc(cc2)CN3CCN(CC3)C(=O)C", 
  "CCCCOc1ccccc1NC(=O)c2cccc(c2)COc3ccccc3C(=O)N", 
  "Cc1c(sc(n1)C)C(C)NC(=O)c2cccc(c2)COc3ccccc3C(=O)N", 
  "CN1CCN(CC1)C(=O)Cc2ccc(cc2)NC(=O)CNc3ccc(cc3)C#N", 
  "CCCCOc1ccccc1NC(=O)c2ccc(cc2)CN3CCN(CC3)C(=O)C", 
  "CCCNC(=O)CCNC(=O)c1ccc(cc1)CN2CCN(CC2)C(=O)C", 
  "CCCNC(=O)CCNC(=O)CNc1ccc(cc1)C#N", 
  "CCCNC(=O)CCNC(=O)c1cccc(c1)COc2ccccc2C(=O)N", 
  "CCCNC(=O)CCNC(=O)c1cnc(s1)c2ccc(s2)Cl", 
  "Cc1cccc(c1)C(=O)NCCNC(=O)CNc2ccc(cc2)C#N", 
  "c1cc(ccc1C#N)NCC(=O)N2CCCC2C3CCCC3", 
  "CC(C)COC(C)C(=O)NCCn1c(=O)n2ccccc2n1", 
  "c1ccn2c(c1)nn(c2=O)CCNC(=O)c3cnc(s3)c4ccc(s4)Cl", 
  "Cc1cc(ccc1OC)NC(=O)c2cc(nc3c2c(nn3c4ccccn4)C)C5CC5", 
  "c1cc(ccc1CCN2CCOCC2)NC(=O)CNc3ccc(cc3)C#N", 
  "c1cc(ccc1CCN2CCCCC2)NC(=O)CNc3ccc(cc3)C#N", 
  "CN1CCC(c2c1cccc2)CNC(=O)CNc3ccc(cc3)C#N", 
  "CC(=O)N1CCN(CC1)Cc2ccc(cc2)C(=O)N3CC4CCC3C4", 
  "CC(C(=O)N1CC2CCC1C2)Oc3ccc(cc3[N+](=O)[O-])Cl", 
  "c1ccc(cc1)n2cc(c(n2)c3ccc(cc3)F)CC(=O)N4CC5CCC4C5", 
  "CC(=O)N1CCN(CC1)Cc2ccc(cc2)C(=O)NCCCCn3ccnc3", 
  "c1ccn2c(c1)nn(c2=O)CCNC(=O)CNc3ccc(cc3)C#N", 
  "Cc1ccc(s1)C(=O)N2CCN(CC2)C(=O)CNc3ccc(cc3)C#N", 
  "Cc1cc(ccc1OC)NC(=O)c2cccc(c2)COc3ccccc3C(=O)N", 
  "c1cc(ccc1C#N)NCC(=O)NCc2cn3cc(ccc3n2)Br", 
  "c1cc(oc1)C(=O)Nc2ccc(cc2Cl)NC(=O)CNc3ccc(cc3)C#N", 
  "C=CCN(Cc1ccccc1F)C(=O)CNc2ccc(cc2)C#N", 
  "Cc1c(cco1)C(=O)N2CCN(CC2)C(=O)CNc3ccc(cc3)C#N", 
  "c1cc(ccc1C#N)NCC(=O)NCCCCn2ccnc2", 
  "c1ccc(c(c1)C(=O)N)OCc2cccc(c2)C(=O)NCCCCn3ccnc3", 
  "c1cc(sc1c2ncc(s2)C(=O)NCCCCn3ccnc3)Cl", 
  "CC(C)(C)OC(=O)NC1CCCN(C1)C(=O)CNc2ccc(cc2)C#N", 
  "CC(=O)N1CCN(CC1)Cc2ccc(cc2)C(=O)N(C)Cc3cccc(c3)C(=O)N", 
  "CC(=O)N1CCN(CC1)Cc2ccc(cc2)C(=O)NCCCc3ccc(cc3)OC", 
  "CC(C(=O)N(C)Cc1cccc(c1)C(=O)N)Oc2ccc(cc2[N+](=O)[O-])Cl", 
  "c1cc(ccc1C#N)NCC(=O)NC(c2ccc3c(c2)OCCO3)C4CC4", 
  "c1ccc(c(c1)CNC(=O)CNc2ccc(cc2)C#N)N3CCCC3", 
  "c1cc(ccc1C#N)NCC(=O)Nc2ccc(cc2)N3CCOC3=O", 
  "CC(C(=O)Nc1ccc(cc1)N2CCOC2=O)OCC3CC3", 
  "Cc1ccn2cc(nc2c1)CNC(=O)CNc3ccc(cc3)C#N", 
  "CC(C)CCC(=O)N1CCN(CC1)C(=O)CNc2ccc(cc2)C#N", 
  "Cc1cc(c(o1)C)c2csc(n2)NC(=O)CNc3ccc(cc3)C#N", 
  "Cc1ccn2cc(nc2c1)CNC(=O)c3cccc(c3)COc4ccccc4C(=O)N", 
  "CC(C(=O)N(C)Cc1cccc(c1)C(=O)N)OCC2CC2", 
  "Cc1c(n2c(n1)CCCC2)CCNC(=O)CNc3ccc(cc3)C#N", 
  "Cc1c(n2c(n1)CCCC2)CCNC(=O)C(C)Oc3ccc(cc3[N+](=O)[O-])Cl", 
  "Cc1c(n2c(n1)CCCC2)CCNC(=O)c3cnc(s3)c4ccc(s4)Cl", 
  "CC(C)(C)c1cc(n[nH]1)NC(=O)CNc2ccc(cc2)C#N", 
  "CC(=O)N1CCN(CC1)Cc2ccc(cc2)C(=O)Nc3cc([nH]n3)C(C)(C)C", 
  "CC(C(=O)Nc1cc([nH]n1)C(C)(C)C)Oc2ccc(cc2[N+](=O)[O-])Cl", 
  "c1ccc(c(c1)c2cc(n[nH]2)NC(=O)CNc3ccc(cc3)C#N)Br", 
  "CC(C(=O)Nc1cc([nH]n1)c2ccccc2Br)OCC3CC3", 
  "CC(C)(CNC(=O)C1CCN(CC1)C(=O)CNc2ccc(cc2)C#N)CN(C)C", 
  "CC(C)COC(C)C(=O)N1CCC(CC1)C(=O)NCC(C)(C)CN(C)C", 
  "c1cc(sc1)CSc2nnc(s2)NC(=O)CNc3ccc(cc3)C#N", 
  "CC(C(=O)Nc1nnc(s1)SCc2cccs2)OCC3CC3", 
  "CC(C(=O)Nc1nnc(s1)c2ccccn2)Oc3ccc(cc3[N+](=O)[O-])Cl", 
  "c1ccnc(c1)c2nnc(s2)NC(=O)c3cnc(s3)c4ccc(s4)Cl", 
  "c1cc(sc1)c2cc(n[nH]2)NC(=O)CNc3ccc(cc3)C#N", 
  "c1cc(ccc1C#N)NCC(=O)N2CCN(CC2)c3ccc(cn3)C#N", 
  "CCOC1CCN(CC1)C(=O)CNc2ccc(cc2)C#N", 
  "CCOC1CCN(CC1)C(=O)c2cccc(c2)COc3ccccc3C(=O)N", 
  "CCOC1CCN(CC1)C(=O)c2cnc(s2)c3ccc(s3)Cl", 
  "CCCC(=O)N1CCCN(CC1)C(=O)CNc2ccc(cc2)C#N", 
  "CCCC(=O)N1CCCN(CC1)C(=O)c2cnc(s2)c3ccc(s3)Cl", 
  "CCCCOc1ccc(c(c1)C)NC(=O)CNc2ccc(cc2)C#N", 
  "CC(C)COc1ccccc1CNC(=O)CNc2ccc(cc2)C#N", 
  "CC(=O)N1CCN(CC1)Cc2ccc(cc2)C(=O)NCc3cccc4c3OCO4", 
  "c1ccc(c(c1)C(=O)NCCNC(=O)CNc2ccc(cc2)C#N)F", 
  "CC(COc1ccc(cc1)F)NC(=O)c2ccc(cc2)CN3CCN(CC3)C(=O)C", 
  "CC(COc1cccc(c1)F)NC(=O)c2ccc(cc2)CN3CCN(CC3)C(=O)C", 
  "CC(Cc1ccsc1)NC(=O)CNc2ccc(cc2)C#N", 
  "CC(Cc1ccsc1)NC(=O)c2ccc(cc2)CN3CCN(CC3)C(=O)C", 
  "COCCOc1ccccc1CNC(=O)CNc2ccc(cc2)C#N", 
  "c1ccc2c(c1)CCN2CC(=O)N3CCN(CC3)C(=O)CNc4ccc(cc4)C#N", 
  "CC1CCCC(C1)(CNC(=O)CNc2ccc(cc2)C#N)N(C)C", 
  "CC(=O)N1CCN(CC1)Cc2ccc(cc2)C(=O)NC3CCCN(C3)C", 
  "CC(C(=O)NC1CCCN(C1)C)Oc2ccc(cc2[N+](=O)[O-])Cl", 
  "CN1CCCC(C1)NC(=O)Cc2cn(nc2c3ccc(cc3)F)c4ccccc4", 
  "CC(=O)N1CCN(CC1)Cc2ccc(cc2)C(=O)N(C)Cc3ccoc3", 
  "CC(C(=O)N(C)Cc1ccoc1)Oc2ccc(cc2[N+](=O)[O-])Cl", 
  "CN(Cc1ccoc1)C(=O)Cc2cn(nc2c3ccc(cc3)F)c4ccccc4", 
  "CC(C(=O)Nc1nc(cs1)c2ccccc2F)OCC3CC3", 
  "Cc1c2c(cc(nc2n(n1)c3ccccn3)C4CC4)C(=O)N(C)CC5CCCCO5", 
  "CC(C(=O)N(C)CC1CCCCO1)Oc2ccc(cc2[N+](=O)[O-])Cl", 
  "CS(=O)(=O)Nc1cccc2c1CCN(C2)C(=O)CNc3ccc(cc3)C#N", 
  "CCOC(=O)N1CCc2c1ccc(c2)NC(=O)CNc3ccc(cc3)C#N", 
  "CCCC(=O)N1CCN(CC1)C(=O)c2cccc(c2)COc3ccccc3C(=O)N", 
  "CCCC(=O)N1CCN(CC1)C(=O)c2cnc(s2)c3ccc(s3)Cl", 
  "c1ccc(c(c1)C(=O)N)OCc2cccc(c2)C(=O)Nc3ccn(n3)CC(=O)N", 
  "CCN1CCCC(C1)NC(=O)c2ccc(cc2)CN3CCN(CC3)C(=O)C", 
  "c1ccc(cc1)n2cc(c(n2)c3ccc(cc3)F)CC(=O)Nc4ccn(n4)CC(=O)N", 
  "CCN1CCCC(C1)NC(=O)c2cc(nc3c2c(nn3c4ccccn4)C)C5CC5", 
  "CCN1CCCC(C1)NC(=O)C(C)Oc2ccc(cc2[N+](=O)[O-])Cl", 
  "Cc1c(nc(s1)CNC(=O)CNc2ccc(cc2)C#N)c3ccccc3", 
  "Cc1c(oc(n1)CNC(=O)C(C)OCC(C)C)C", 
  "Cc1c(oc(n1)CNC(=O)c2cccc(c2)COc3ccccc3C(=O)N)C", 
  "Cc1c(nc(s1)CNC(=O)C(C)OCC2CC2)c3ccccc3", 
  "Cc1c(oc(n1)CNC(=O)C(C)Oc2ccc(cc2[N+](=O)[O-])Cl)C", 
  "Cc1c(oc(n1)CNC(=O)Cc2cn(nc2c3ccc(cc3)F)c4ccccc4)C", 
  "COC(=O)CC1CCN(CC1)C(=O)c2cccc(c2)COc3ccccc3C(=O)N", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)CCc3[nH]c4ccccc4c(=O)n3", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)Cn3c(=O)c4ccccc4c(=O)[nH]3", 
  "CCn1c(=O)c2ccccc2c(n1)C(=O)Nc3ccc(cc3)NC(C)c4ccccc4", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)CCC(=O)c3ccccc3", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)CCc3ccc(c(c3)OC)OC", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)C(C)N3C(=O)C4CC=CCC4C3=O", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)C3CCCO3", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)c3ccc(cc3)n4cnnn4", 
  "Cc1c(c(on1)C)C(=O)Nc2ccc(cc2)NC(C)c3ccccc3", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)C3CC(=O)N(C3)Cc4ccco4", 
  "Cc1c(c(n(n1)CC(=O)Nc2ccc(cc2)NC(C)c3ccccc3)C)[N+](=O)[O-]", 
  "Cc1cccc(c1)C(=O)NC(C)C(=O)Nc2ccc(cc2)NC(C)c3ccccc3", 
  "CCCn1c(=O)ccc(n1)C(=O)Nc2ccc(cc2)NC(C)c3ccccc3", 
  "CCCCn1c(c(c(n1)C)C(=O)Nc2ccc(cc2)NC(C)c3ccccc3)Cl", 
  "Cc1ccc(cc1C(=O)Nc2ccc(cc2)NC(C)c3ccccc3)S(=O)(=O)C", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)C3CCN(CC3)C(=O)C4CC4", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)c3ccc4c(c3)NC(=O)CCS4", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)c3cccc(c3)NC(=O)C4CC4", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)c3cc(cc(c3)[N+](=O)[O-])[N+](=O)[O-]", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)c3cccnc3", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)c3cc(=O)c4ccccc4o3", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)Cc3ccc(cc3)OC", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)Cc3ccsc3", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)Cc3ccc(c(c3)OC)OC", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)c3ccc[n+](c3)[O-]", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)COc3ccccc3C#N", 
  "Cc1cc(n2c(n1)nc(n2)C(=O)Nc3ccc(cc3)NC(C)c4ccccc4)C", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)C3=COCCO3", 
  "CC(c1ccccc1)N=C2C=CC(=NC(=C3C=CN(C=C3)O)O)C=C2", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)CN3C(=O)C4CCCCC4C3=O", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)CN3C(=O)C(NC3=O)(C)C4CC4", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)Cn3ccccc3=O", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)CCc3ccccc3O", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)CCNC(=O)c3cccs3", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)CCCNC(=O)c3ccccc3", 
  "Cc1c(c(on1)C)CC(=O)Nc2ccc(cc2)NC(C)c3ccccc3", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)c3ccc4c(c3)NC(=O)CO4", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)c3ccc(c(c3)F)OC", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)CN3c4ccccc4SCC3=O", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)CCn3c4ccccc4oc3=O", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)CCc3nc(no3)c4ccccc4", 
  "Cc1cc(on1)C(=O)Nc2ccc(cc2)NC(C)c3ccccc3", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)CNC(=O)c3ccccc3", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)CNC(=O)c3ccc(cc3)OC", 
  "Cc1cccc(c1)C(=O)NCC(=O)Nc2ccc(cc2)NC(C)c3ccccc3", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)CCNC(=O)OC(C)(C)C", 
  "CCOC(=O)Nc1cccc(c1)C(=O)Nc2ccc(cc2)NC(C)c3ccccc3", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)C3CCCN(C3)C(=O)C", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)CCc3nc(no3)c4ccsc4", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)C3CCS(=O)(=O)C3", 
  "Cc1c(cc(cn1)[N+](=O)[O-])C(=O)Nc2ccc(cc2)NC(C)c3ccccc3", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)c3ccc(s3)C(=O)C", 
  "Cc1c(cc(o1)CCC(=O)Nc2ccc(cc2)NC(C)c3ccccc3)C(=O)C", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)c3ccc(cc3)OCC(=O)N", 
  "Cc1nc(cs1)CSCC(=O)Nc2ccc(cc2)NC(C)c3ccccc3", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)CCc3nc(no3)c4cccs4", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)C3=NN(C(=O)CC3)C", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)c3ccc(cc3)CCNC(=O)C", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)c3ccc4c(c3)CCC(=O)N4", 
  "Cc1c(sc(=S)[nH]1)CC(=O)Nc2ccc(cc2)NC(C)c3ccccc3", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)c3cc(ccc3[N+](=O)[O-])SC", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)CC3C(=O)Nc4ccccc4S3", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)C3CCN(CC3)C(=O)C(C)(C)C", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)c3cc(cn3C)S(=O)(=O)NC", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)CCn3cnc4ccccc4c3=O", 
  "Cc1c(c(n(n1)C)C)CCC(=O)Nc2ccc(cc2)NC(C)c3ccccc3", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)CN3C(=C)c4ccccc4C3=O", 
  "CC(C)n1c2c(cc(cn2)C(=O)Nc3ccc(cc3)NC(C)c4ccccc4)cn1", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)CCc3ccco3", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)Cn3cnc4c(c3=O)cnn4C", 
  "Cc1c(c(=O)[nH]c(=O)[nH]1)CCC(=O)Nc2ccc(cc2)NC(C)c3ccccc3", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)C(C)N3C(=C)c4ccccc4C3=O", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)Cc3ccc4c(c3)CCO4", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)CCc3ncc(o3)c4ccccc4", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)CNC(=O)c3ccc(cc3F)F", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)CCn3cnc4c(c3=O)ccs4", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)Cn3c4ccccc4nn3", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)COc3ccc(cc3)C(=O)C", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)c3ccc(o3)Cn4ccccc4=O", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)c3ccc(nc3)n4ccnc4", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)c3ccc(nc3)C(F)(F)F", 
  "Cc1nc(on1)c2ccc(cc2)C(=O)Nc3ccc(cc3)NC(C)c4ccccc4", 
  "Cc1c(c(on1)C(C)C)C(=O)Nc2ccc(cc2)NC(C)c3ccccc3", 
  "CCc1c(snn1)C(=O)Nc2ccc(cc2)NC(C)c3ccccc3", 
  "CC(c1ccccc1)Nc2ccc(cc2)NC(=O)c3ccc(nc3)n4cccn4", 
  "CCOC(=O)C(CCC1=CC=CC=C1)NC(C)C(=O)N2CCCC2C(O)=O",
  "COC1=C(O)C=C(O)C2=C1OC(CC2=O)C1=CC=CC=C1",
  "C1=CC=C(C=C1)C2=CC=NC=C2",
  "O=C1NC(=O)C2=C(C=CC=C2)C1=CC1=CC=CC=C1",
  "CN1C(=O)N(C)c2cc(ccc12)-c3[nH]c(nc3-c4ccc(F)cc4)-c5ccc(cc5)S(C)=O",
  "[Na]Cl",
  "CC1=NC2=C(C=CC=C2)N1C(=O)C1=CC(Cl)=CC=C1Cl",
  "C1=CC2=NC=CN2N=C1",
  "N1C2=C(SC3=C1C=CC=C3)C=CC=C2",
  "NC1CCNCC1N1CCCC1=O",
  "CC(=C)C1=C(C)C=C(C(C)=C1)C2=CC3=C(C=CC=C3)C=C2",
  "NC(=N)c1ccc(NC(=O)c2ccccc2O)cc1",
  "Cc1nc(Nc2ncc(s2)C(=O)Nc3c(C)cccc3Cl)cc(n1)N4CCN(CCO)CC4",
  "NNC1=CSC=N1",
  "NC1=NC2=CC=CC=C2N1",
  "O=S(=O)(NC1=NC=CC=C1)C2=CC=CC=C2",
  "CN1CCCC1CC2=CC=CC=C2",
  "C1CCC(=CC1)N1CCOCC1",
  "O1C=NN=C1",
  "CC(C)[C@H](NC(=O)CON=C1/CC[C@]2(C)[C@@H]3CC[C@@]4(C)[C@@H](CC[C@]4(O)C#C)[C@H]3CCC2=C1)C(=O)N[C@@H](CC(O)=O)C(O)=O",
  "SCCCC(=O)NC1CCN(CC2=CC=CC=C2)CC1",
  "CS(=O)(=O)N1CCC2=C(C1)C=CS2",
  "COC1=CC=C(C=C1)S(=O)(=O)C(CC2=CC=CC=N2)C(C(C)C)C(=O)NO",
  "O=C(NC1=NC(=CS1)C1=NC=CC=C1)C1=CC=CC=C1",
  "S=C1NC=NN1",
  "O=C(O)CCc1cc(O)cc(O)c1",
  "CC1=C(CCC(=O)N[C@H](CSCC2=CC=CC=C2)C(=O)NCCCC(O)=O)C(=O)OC2=CC3=C(C=C12)C1=C(CCCC1)O3",
  "C1CNC2=CC=CC=C2C1",
  "O=C(NCC1=CC=CC=C1)C2=CC=CN2",
  "CC1CN(CC(C)N1)C2=CC3=C(C=C2F)C(=O)C(=CN3C(C)(C)C)C(O)=O",
  "CC1CCCC1CCC2CCCC3CCCCC23",
  "CC1=C(C)N2C=CN=CC2=N1",
  "CN1CCN(CC1)C1=CC=C2[N]C(=NC2=C1)C1=C2C=CC=CC=C2NC1=O",
  "N#CC(C#N)=C1C=CC=CC=C1",
  "C[C@@H]1OC[C@H](N)[C@@H]1O",
  "COC1=CC=C(C=C1)C2=C(CCNS(=O)(=O)N[C@@H]3CCN(CC4=CC=CC=C4)C3)N=C(N2)C5=CC=CC=C5",
  "COC1=CC(Br)=CC(C(=O)NC2=CC=C(F)C=C2)=C1OC",
  "NC1=NC2=C(C(=O)N1)C(CCNNCC3=CN(C4OC(CO)CC4O)C5=C3C(=O)NC(N)=N5)=CN2C6CC(O)C(CO)O6",
  "C1CC2CCCC2C1",
  "CC1=NC(CN)=C(N)O1",
  "C1CCC(CC1)NC2=C3N=CNC3=NC=N2",
  "CN1C=NN=C1C",
  "O=C1NC2=C(C3=C(CCCC3)S2)C(=O)N1",
  "CC(C)(CO)CC1=CC(=C(O)C(=C1)C(C)(C)C)C(C)(C)C",
  "[H]N([H])[C@@]1([H])C([H])([H])C([H])([H])[C@]([H])(N([H])C2=NC(N([H])C([H])([H])C3=C([H])C([H])=C([H])C([H])=C3[H])=C4N=C([H])N(C4=N2)C5([H])C([H])([H])C([H])([H])C([H])([H])C5([H])[H])C([H])([H])C1([H])[H]",
  "CC(C1=CC=CC=C1)C2=CC=CC=C2",
  "NC1=C2CCCCC2=NC3=CC=CC=C13",
  "CC1CCN(C)CC1",
  "NC1=C(C=C(C=C1)N(=O)=O)C(O)=O",
  "CCCCCCCCCCCCCCCC(O)=O",
  "N[C@H]1CC=CCC1C2=CC=CC=C2Cl",
  "O=C1NC(=O)N(CC2=CC=CC=C2)C=C1",
  "CNCCCC",
  "O=C(NN=CC1=CC=CC=C1)C2=NOC=C2",
  "OC1=CN=CC2=C1C=CC=C2",
  "CCN1CCCC(O)C1",
  "C1CC(CCN1)C2=CC=CC=C2",
  "CN1C(=O)N(C)c2ncn(C)c2C1=O",
  "CC1=CC2=C(C=C1)C(Cl)=CC(=C2)C3=CC(Cl)=CC(C)=C3",
  "c1cc(N)ccc1N",
  "CC1=CC2=CC3=C(OCCO3)C=C2CN1",
  "CC1C=C(NC(=O)C2=CC(NC(=O)C3=CC(=CC3C)N(C)C(=O)c4ccc(cc4)C(=O)N(C)C5=CC(C)C(=C5)C(=O)NC6=CC(C)C(=C6)C(=O)NC7=CC(C)C(=C7)C(=O)NCCCN(C)C)=CC2C)C=C1C(=O)NCCCN(C)C",
  "C1CC(CCN1)(c2ccccc2)c3ccc(cc3)-c4cn[nH]c4",
  "NN1CC(=O)NC1=O",
  "CN(C1CCCN2C(=O)C(O)=C(N=C12)C(=O)NCc3ccc(F)cc3)S(=O)(=O)N4CCN(C)CC4",
  "O=C1NC2=CC=C3N=CSC3=C2\\C1=C\\NC1=CC=CC=C1",
  "O[C@@H]1CC[C@]2(O)C[C@H]1OC2=O",
  "Oc1ncnc(C(=O)NCc2ccc(F)cc2)c1O",
  "O(C1=CC=CC=C1)C2=CC=CC=C2",
  "C1CC2=CC=CC=C2CN1",
  "COC",
  "O=C1CCCCO1",
  "OCC1CC(O)C(O)CO1",
  "C1=CN=NN=C1",
  "CNC(=O)COC(=O)NCC1=CC=C(C=C1)C2=CC=CC=C2",
  "O=C1NCC(NS(=O)(=O)C2=CC=CC=C2)C1CNCC3CC3",
  "N1NC(C2=CNC=C2)C(=C1)C3=CC=CC=C3",
  "[H]OC1=C([H])C2=C(C([H])=C1[H])[C@@]3([H])C([H])([H])C([H])([H])[C@]4(C([H])([H])[H])[C@@]([H])(O[H])C([H])([H])C([H])([H])[C@@]4([H])[C@]3([H])C([H])([H])C2([H])[H]",
  "C1=CN=C2C(=C1)C=CC3=CC=CN=C23",
  "CNC(=O)C1=CNC2=C1C=CC=C2",
  "[H]N1C(=O)CCCCC1([H])[H]",
  "OC[C@H]1O[C@@H]2O[C@H]3[C@H](O)[C@@H](O)[C@H](O[C@@H]3CO)O[C@H]4[C@H](O)[C@@H](O)[C@H](O[C@@H]4CO)O[C@H]5[C@H](O)[C@@H](O)[C@H](O[C@@H]5CO)O[C@H]6[C@H](O)[C@@H](O)[C@H](O[C@@H]6CO)O[C@H]7[C@H](O)[C@@H](O)[C@H](O[C@@H]7CO)O[C@H]1[C@H](O)[C@H]2O",
  "S1C=NC=C1N1C=NC2=CC=CC=C12",
  "[H]\\C",
  "CC1(COC(OC1)C2=NC(C3=CC=NC(N)=N3)=C(N2)C4=CC=C(F)C=C4)C(=O)N5CCOCC5",
  "SC1=NC=CC=C1",
  "CC1=C(C#N)C2=C(C=CC=C2)C=N1",
  "O=C(NC1=CC2=C(C=NN2)C=C1)C1=CC2=C(C=C1)N=CN2",
  "C1NC2=C(C=NC(NC3=CC=CC=C3)=N2)C=C1C4=CC=CC=C4",
  "COC(=O)CNC(=O)C1=CC(=O)OC2=CC=CC=C12",
  "[H]N(CC)CC",
  "SC1=NC=CC=C1Cl",
  "CCOC(=O)C1=NOC(=C1)C2=CC=C(OC(C)(C)C)C(=C2)N(=O)=O",
  "[H]N1C(Br)=C([H])C2=C1C(=O)N([H])C([H])([H])C=C2",
  "O=CN1CCCC1C",
  "CCC",
  "CCC=C",
  "C1(=NN=C(S1)CCC2=CC=CC=C2)NC(=O)C3=CC=CC=C3",
  "SC1=CC=CC=C1Br",
  "[H]C1=NC2=CC=CC=C2C(NC3=CC=CC=C3)=C1",
  "NC1=NC=CC(=N1)N2C=NC3=CC=CC=C23",
  "C1CCC(CC1)C1CCCCC1",
  "[H]C(=O)C(O)COP([O-])([O-])=O",
  "[O-]O[S](N1CCNCC1)C2=CC=CC3=C2C=CC=C3Cl",
  "COC(=O)NC1=CC2=C(C=C1)C3=CC=C(C=C3O2)S(=O)(=O)NC(C(C)C)C(O)=O",
  "CC(=O)C1=C(O)C=C(O)C=C1",
  "CC1=NOC2=C1C(=O)NN=C2CC3=CC=CC=C3",
  "CC1(COC(OC1)C2=NC(C3=CC=NC(NC4CC4)=N3)=C(N2)C5=CC=C(F)C=C5)C(=O)N6CCOCC6",
  "C1CCC2OCCCC2C1",
  "NC1=C(SC(NC2=CC=C(C=C2)N2CCNCC2)=N1)C(=O)C1=C(F)C=CC=C1F",
  "CC(C)CC1=CC=CC=C1",
  "C1=CC2=C3C(C=CC4=CC=CC(C=C2)=C34)=C1",
  "CCCCO",
  "Cc1ccc(cc1)S(=O)(=O)OCC(=O)Nc1ccc(C(O)=O)c(O)c1",
  "OC(=O)C1=CC(=O)OC2=C1C=CC=C2",
  "BrC1=CC=CC=C1",
  "FC1=CC=CC(F)=C1F",
  "O=C1CC2SCC=CC12",
  "C1C2=CNN=C2C3=CC=CC=C13",
  "CC(C)(C)N1CC(=O)N=CC2=CC(N)=CC=C12",
  "ClC1=CC=C(C=C1)S(=O)(=O)NC2=CC(Cl)=CC(Cl)=C2",
  "C[C@H](NC1=NC(=O)[C@](C)(S1)C(F)(F)F)c2ccccc2F",
  "[H]N=C(N([H])C)N([H])C",
  "FC1=CC=C(C=C1)C(COC=O)C2CCC2",
  "CCN(CC)CCC1=CC=C(CNC(=O)NC2=CC(OC)=CC=C2OC)O1",
  "CN1C(CCCC1CC(O)C2=CC=CC=C2)CC(O)C3=CC=CC=C3",
  "C[C@@]12CCCC1C3CCC4CCCCC4C3CC2",
  "COC1=CC(=CC(OC)=C1OC)[C@H]1[C@@H]2[C@H](COC2=O)C(O)C2=CC3=C(OCO3)C=C12",
  "OC1=CC=C(C=C1)C2=COC3=CC(O)=CC(O)=C3C2=O",
  "C1CC2CCC3C(CCC4=C3C=CC=C4)C2C1",
  "NC(=N)C1=CC=C(C=C1)C2=CC=C(O2)C3=CC=C(C=C3)C(N)=N",
  "C1CN=C(NN=CC2=CC=CC=C2)N1",
  "COP(=O)(OC)OC1=CC=CC=C1",
  "C1=CN=CN=C1",
  "OC(=O)P(O)(O)=O",
  "O=CCC1=CC=CC=C1",
  "CCCN",
  "CNC(=O)NS(=O)(=O)C1=CC=CC=C1",
  "CC1C2=CC=CC=C2CCC3=C1N=CC=C3",
  "NC1(CCCC1)C2=NC3=NC=NC=C3N=C2",
  "CN1C=C(C)C(C)=C1C",
  "O1C=CC2=C1C=NC=N2",
  "OC(=O)c1ccc(NC(=O)c2ccccc2)cc1",
  "NC1=NC=NC2=C1N=CN2",
  "BrC1CCCC1",
  "CC1=NC2=C(C)C=CC=C2N1",
  "CF",
  "CN1CCOc2cc3NC(=O)C=C(c3cc12)C(F)(F)F",
  "CC(=O)C1(C)NCC(C)(C)O1",
  "[H]O[C@]([H])(C([H])([H])N(C([H])([H])C([H])(C([H])([H])[H])C([H])([H])[H])S(=O)(=O)C1=C([H])C([H])=C(N([H])[H])C([H])=C1[H])[C@@]([H])(N([H])C(=O)O[C@]2([H])C([H])([H])OC([H])([H])C2([H])[H])C([H])([H])C3=C([H])C([H])=C([H])C([H])=C3[H]",
  "CN1C(=O)N(C2=C1C=NC3=CC=C(C=C23)C4=CC5=CC=CC=C5N=C4)C(C)(C)C#N",
  "OC(=O)C1=CC2=CC3=C(C=CC=C3)C=C2OC1=O",
  "C1NOC=C1",
  "CC12CCCC1C3CCC4=CCCCC4C3CC2",
  "[H]C1=C([H])C(=NC(NC)=N1)C2=CC=CC=C2",
  "C1CC2CCC3=C(C=CC4=CC=CC=C34)C2C1",
  "N[C@@H](CC(=O)N1CCn2c(C1)nnc2C(F)(F)F)Cc3cc(F)c(F)cc3F",
  "Oc1nc(Cc2ccccc2)nc(C(=O)NCc3ccc(F)cc3)c1O",
  "NC1=CC=CC=N1",
  "CC(C)(C)c1ccc(cc1)S(=O)(=O)N[C@@H](Cc2cccc(c2)[CH+](N)N)C(=O)N3CCN(CC3)S(C)(=O)=O",
  "OC(=O)CN1C(=O)N(CCCC2=CC=CC=C2)C2=C(C3=C(CCC3)S2)C1=O",
  "CC1CCC=CC(C)=CC=CC=C1",
  "CC1CCCCC1OC2=C(O)C(=CC=C2)C3=CC4=CC(C(N)=N)=C(F)C=C4N3",
  "CC1=CC(NC2=CC=NC(NC3=CC=C(N)C=C3)=N2)=NN1",
  "N[C@H]1COC(=C)[C@H]1O",
  "OC(=O)CCCNC(=O)[C@@H](CSCC1=CC=CC=C1)NC=O",
  "CC1=C(SC(=N1)[C@@H]2COC3=C(O2)C=CC=C3)C(O)=O",
  "OCCCNC1=NC=CC(=C1)C1=NC(NC2=CC=CC(Cl)=C2)=NC=C1",
  "C#C",
  "N1C=C(C2=CC=NN2)C3=C1C=CC(=C3)C4=CC=CC=C4",
  "COC1=C(O)C=C2N=CN=C(N)C2=C1",
  "CCCCCN1C=C(C(=O)C2=CC=CC3=CC=CC=C23)C4=CC=CC=C14",
  "CC(C)(N)CC1=CC=CC=C1",
  "O=C1NC2=CC=CC(OC3CCCCC3)=C2C=C1",
  "OC1=CC=C(C=C1)C2=NC(NCCC3=CC=CC=C3)=CC(=C2)C4=CC=CC=C4",
  "CC1=CNC=C1",
  "N[C@H](CC1=CC2=C(N1)C=CC=C2)C(N)=O",
  "CN(CCCC1=CC=C(CC2CC(=O)CC2=O)C=C1)C3=CC=CC=C3",
  "[H:43][N:14]([c:15]1[n:16][c:17]([H:44])[c:18]([o:19]1)-[c:20]1[c:21]([H:45])[c:22]([H:46])[c:23]([H:47])[c:24]([c:25]1[H:48])-[c:26]1[n:31][c:30]([H:52])[c:29]([H:51])[c:28]([H:50])[c:27]1[H:49])[c:12]1[c:13]([H:42])[c:6]([c:7]([H:37])[c:8]([H:38])[c:9]1[O:10][C:11]([H:39])([H:40])[H:41])[S:2](=[O:1])(=[O:3])[C:4]([H:32])([H:33])[C:5]([H:34])([H:35])[H:36]",
  "CC1=C(C=NN1)C1=CC=CC=C1",
  "O=C=NC1=CC=CC=C1",
  "CCC1=C(C)NC(=O)C(CC2=NC3=C(O2)C=CC=C3)=C1",
  "CCC1=C(OC2=CC(=C(C=C2)C#N)C(F)(F)F)C=CC=C1",
  "PC1=CC=CC=C1",
  "OCC(O)C(O)C1=CNC2=CC=CC=C12",
  "CC1=CC=CS1",
  "CC1NCC(N)C1O",
  "ONC(=O)CCCP(O)(O)=O",
  "NC(=O)C1=CC(=NN1)C(F)(F)F",
  "CC1=C(C)C(C)=C(C)N1",
  "[H]C(C)=NO",
  "CNC(=O)C1=C(C)NC(C=C2/C(=O)NC3=CC=CC=C23)=C1C",
  "OC(=O)[C@H](Cc1ccc(NC(=O)c2c(Cl)cncc2Cl)cc1)NC(=O)[C@@H]3C4CCC(CC4)N3C(=O)CCC5CCCC5",
  "N1C=CN=C1",
  "CC#N",
  "C1=CC=C(C=C1)N2C=CC=C2",
  "OC1=CC(=CC=C1)N2CCCC2",
  "NC(=O)C1=CC=CN=C1NC1=NC(=CC2=NC=CN12)C1=CC=CC=C1",
  "CC(=NN)C(C)=N/O",
  "C[As+](C)(C)CCO[C@@H](C1=CC=CC=C1[N+](O)=O)C(F)(F)F",
  "CCN1C=C(C(=C1)C2=CC=CC=C2)C3=CC=CC=C3",
  "CC1=CC(NC2=NC=CC(=N2)N3C=CN=C3C4=CC=CC=C4)=CC(C)=C1",
  "CCCCCOP(O)(O)=O",
  "NS",
  "O=C1NC=CC(CC2=CC=CC=C2)=C1",
  "CNC(=O)OC1=CC=CC(O)=C1O",
  "CN1C(=O)N(C)c2nc[nH]c2C1=O",
  "C1CCC2=C(C1)NC3=C2C=CC=C3",
  "CCCC(C)C1=C2C=C(C)C(C)=CC2=CC3=C1NC4=CC=CC=C34",
  "[H]C1=NC(N)=NC(N)=C1[H]",
  "OC1CCCN(C1)CC2=CC=CC=C2",
  "CN1CCOCC1C2=NC(C(=O)NCc3ccc(F)cc3)=C(O)C(=O)N2C",
  "CI",
  "CP(S)(=O)C1=CC=C(N)C=C1",
  "O=CN1CCC1",
  "CC(=O)N1Cc2[nH]nc(NC(=O)c3ccc(F)cc3)c2C1",
  "CC(N)C(O)NCC(O)=O",
  "CN(C)C(C)=O",
  "NC(=O)C1=CC=C(OC2=CC=CC=C2)C=C1",
  "C1CC2=C(C1)C3=C(C=C2)C4=CC=CC=C4C=C3",
  "CCn1cc(C(=O)C(N)=O)c2ccccc12",
  "CN1C=C(C(C)=C1C)C1=CC=CC=C1",
  "C1CC2=C(NC3=CC=CC=C23)C(N1)C4=CC=CC=C4",
  "N(C1=NC(=CS1)C1=NC=CC=C1)C1=CC=CC=C1",
  "CC(=O)NC1(CC2=CC=CC=C2C1)NC(C)=O",
  "c1ccccc1O",
};

void check_contains_no_whitespace(char *compressed_smiles, int compressed_len,
                                  char *uncompressed_smiles, int uncompressed_len) {
  int offset;
  for (offset=0; offset<compressed_len; offset++) {
    switch (compressed_smiles[offset]) {
    case '\r':
    case '\n':
    case '\t':
    case ' ':
      printf("whitespace compression error at %d: %*s\n", offset, uncompressed_len, uncompressed_smiles);
      for (offset=0; offset<compressed_len; offset++) {
        printf(" [%d] %d\n", offset, ((unsigned char *) compressed_smiles)[offset]);
      }
      exit(1);
    default:
      break;
    }
  }
}


int test_compression(int dictionary) {
  char *smiles;

  char compressed_smiles[2000];
  char uncompressed_smiles[2000];
  int len = 2000;
  int result;
  unsigned int smilesno = 0;
  int uncompressed_len = 0;
  int compressed_len = 0;
  
  for (smilesno=0; smilesno<sizeof(smiles_list)/sizeof(char *); smilesno++) {
    smiles = (char *) smiles_list[smilesno];
    /*printf("%s\n", smiles);*/
    uncompressed_len += strlen(smiles);
    result = smilez_compress(smiles, strlen(smiles), compressed_smiles, len, dictionary);
    if (dictionary != 0) {
      check_contains_no_whitespace(compressed_smiles, result, smiles, strlen(smiles));
    }
    compressed_len += result;
    result = smilez_decompress(compressed_smiles, result, uncompressed_smiles, 2000);
    uncompressed_smiles[result] = 0;
    if (result != (int) strlen(smiles)) {
      printf("Oops?\n%s\n%s\n", smiles, uncompressed_smiles);
      exit(1);
    }
    if (strcmp(smiles, uncompressed_smiles) != 0) {
      printf("Oops!\n%s\n%s\n", smiles, uncompressed_smiles);
      exit(1);
    }
  }

  printf("Uncompressed %d Compressed %d Ratio %f\n", uncompressed_len, compressed_len,
         compressed_len / (0.0 + uncompressed_len));
  return 0;
}

void check_reverse(char *compressed_smiles, int len, int dictionary) {
  char uncompressed_smiles[100];
  char recompressed_smiles[100];
  char reuncompressed_smiles[100];
  int uncompressed_len1, uncompressed_len2;
  int compressed_len;

  uncompressed_len1 = smilez_decompress(compressed_smiles, len, uncompressed_smiles, 100);
  compressed_len = smilez_compress(uncompressed_smiles, uncompressed_len1, recompressed_smiles, 100,
                                   dictionary);
  if (dictionary != 0) {
    check_contains_no_whitespace(recompressed_smiles, compressed_len,
                                 uncompressed_smiles, uncompressed_len1);
  }
  uncompressed_len2 = smilez_decompress(recompressed_smiles, compressed_len, reuncompressed_smiles, 100);
  
  if (uncompressed_len1 != uncompressed_len2) {
    printf("Length mismatch: %d != %d\n%*s\n%*s\n", uncompressed_len1, uncompressed_len2,
           uncompressed_len1, uncompressed_smiles,
           uncompressed_len2, reuncompressed_smiles);
  }
  if (memcmp(uncompressed_smiles, reuncompressed_smiles, uncompressed_len1)) {
    printf("Value mismatch: %d\n%*s\n%*s\n", uncompressed_len1, 
           uncompressed_len1, uncompressed_smiles,
           uncompressed_len2, reuncompressed_smiles);
  }
}

void test_reversability(int dictionary) {
  char compressed_smiles[10];
  int c1, c2, c3;

  for (c1=0; c1<254; c1++) {
    compressed_smiles[0] = c1;
    check_reverse(compressed_smiles, 1, dictionary);

    for (c2=0; c2<254; c2++) {
      compressed_smiles[1] = c2;
      check_reverse(compressed_smiles, 2, dictionary);

      for (c3=0; c3<254; c3++) {
        compressed_smiles[2] = c3;
        check_reverse(compressed_smiles, 3, dictionary);
      }
    }
  }
}


int main(void) {
  printf("Testing byte compression\n");
  test_compression(0);
  printf("Testing whitespace compression\n");
  test_compression(1);
  printf("Testing byte reversability\n");
  test_reversability(0);
  printf("Testing whitespace reversability\n");
  test_reversability(1);
  return 0;
}
