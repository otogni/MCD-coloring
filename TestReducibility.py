# coding: utf-8
# python functions to test if handles are (9,4)-reducible
# see the paper "New reducible configurations for graph multicoloring with application to the experimental resolution of McDiarmid-Reed's Conjecture"  by Jean-Christophe Godin and Olivier Togni

from itertools import chain, combinations, product, permutations
import sys
from random import randint

T={1,2,3,4,5,6,7,8,9}	# the nine colors used
X2=[1,2,12,47,122]	# the indexes of the 4-color subsets used on Port 2 if port one has the color subset n°1
X3=[[1,2,12,47,122],[1,2,3,7,8,12,16,37,41,47,53,112,118,122,126],[1,2,4,12,13,19,22,24,27,28,34,47,50,56,92,93,99,102,105,111,122,125],[1,2,5,12,14,21,47,48,52,57,60,62,64,71,82,83,87,122,124],[1,2,6,12,15,47,49,122,123]]	# the set of 4-color subsets used on Port 3, depending on the set assigned to Port 2
I=range(1,127)
P=list(combinations(T, 4))	# the 126 4-color subsets
D={i:s for i,s in zip(I,P)}	# dictionnary of the 126 4-color subsets
Dp={s:i for i,s in zip(I,P)}	# reverse dictionnary


	
# for a path of length a1+a2 with extremities v1 and v2 and a vertex v along the path at distance a1 from v1 and a2 from v2, 
# the function returns the list of the indexes of the 4-colors subsets that can be given to v assuming v1 is colored by subset i1 and v2 by subset i2
def path(a1,a2,i1,i2):
	L=[]
	for i in I:
		if compat(a1,i1,i) and compat(a2,i2,i):
			L.append(i)
	return L
	
	
# test if 4-colors subsets i1 and i2 can be given to vertices at distance l (see definition of l-compatibility in the paper)
def compat(l,i1,i2):
	if l>7: return True
	S1=set(D[i1]); S2=set(D[i2])
	if l%2==0 and len(S1 & S2)>= 4-l/2: return True
	if l%2==1 and len(S1 & S2)<=(l-1)/2: return True
	return False


# filter the list L by keeping only the 4-color subsets that are d-compatible with the subset 1
def filter(L,d):
	M=[]
	for i in L:
		if compat(d,i,1): M.append(i)
	return M


# test if two lists of 4-subsets indexes are l-compatible, i.e. if there is a subset in L1 that is l-compatible with one in L2
def compatSet(l,L1,L2):
	for i in L1:
		for j in L2:
			if compat(l,i,j):  return True 
	return False

	
# test if LLL is not a surlist of one of the lists of S	and filter S by removing its items that are surlists of LLL
def isNotSuperSet(LLL,S):  
	find=True
	for s in S:
		if LLL >= s: find=False; break
		if s > LLL: S.remove(s) ; 
	return find
	

# return the string version of the 4-color set D	
def CharD(D):
	S="" 
	for i in D:
		S=S+str(i)
	return S
	


# return a string version of the list s	of 4 color subsets
def printDs(s):
	sp=[]
	for x in s:
		sp.append(CharD(D[x]))
	return sp
		

	
# compute the list of possible 4-color subsets on vertex v in the central path of a H-handle H(a,b,c1+c2,d,e) 
# at distance c1 from v1 (that has two other outgoing paths of length a and b) and c2 from v2 (that has two other outgoing paths of length d and e)
# when the endvertices of the paths of length a, b, d, e are colored by subsets i1, j1, i2, j2, respectively
def HgraphFixed(a,b,c1,c2,d,e,i1,j1,i2,j2):
	S1=[]; S2=[]; S3=[]
	S1=path(a,b,i1,j1)
	S2=path(d,e,i2,j2)
	
	for k in S1:
		for l in S2:
			Pa=path(c1,c2,k,l)
			if len(Pa): S3.append(Pa)
	return {j for x in S3 for j in x}
					
	
# compute the list of lists of possible 4-color subsets on vertex v in the central path of a H-handle H(a,b,c1+c2,d,e) 
def Hgraph1Fixed(a,b,c1,c2,d,e): 
	S=[]; 
	tt=0; mint=50; minl=50;
	for cpt in range(0,5):
		i=X2[cpt]; XX3=X3[cpt];
		if compat(a+b,1,i): 
			for j in XX3:
				if compat(a+c1+c2+d,1,j) and compat(b+c1+c2+d,i,j): 
					for k in I:
						if compat(d+e,j,k) and compat(a+c1+c2+e,1,k) and compat(b+c1+c2+e,i,k):
							LLL=HgraphFixed(a,b,c1,c2,d,e,1,i,j,k)
							if len(LLL)>0 and isNotSuperSet(LLL,S):
								S.append(LLL);
								
	return S
		

# test if a cyclic configuration is reducible. The cycle C consists of paths of length ab, bc, cd, da with outgoing paths of length a, b, c, d
def testCycle(a,b,c,d,ab,bc,cd,da): 
  for cpt in range(0,5):
    i=X2[cpt]; XX3=X3[cpt];
    for j in XX3:
      if compat(a+c+cd+da,1,j) and compat(b+c+bc,i,j): 
          for k in I:
            if compat(a+d+da,1,k) and compat(c+d+cd,j,k) and compat(b+d+bc+cd,i,k):
              S1=[]; S2=[]; S3=[]; S4=[]
              S1=set(path(a,da+d,1,k)) & set(path(a,ab+bc+cd+d,1,k)) & set(path(a,da+cd+bc+b,1,i)) & set(path(a,ab+b,1,i)) & set(path(a,da+cd+c,1,j)) & set(path(a,ab+bc+c,1,j))
              S2=set(path(b,bc+c,i,j)) & set(path(b,ab+da+cd+c,i,j)) & set(path(b,bc+cd+d,i,k)) & set(path(b,ab+da+d,i,k)) & set(path(b,ab+a,i,1)) & set(path(b,bc+cd+da+a,i,1))
              S3=set(path(c,cd+d,j,k)) & set(path(c,bc+ab+da+d,j,k)) & set(path(c,cd+da+a,j,1)) & set(path(c,bc+ab+a,j,1)) & set(path(c,bc+b,j,i)) & set(path(c,cd+da+ab+b,j,i))
              S4=set(path(d,da+a,k,1)) & set(path(d,cd+bc+ab+a,k,1)) & set(path(d,ab+da+b,k,i)) & set(path(d,bc+cd+b,k,i)) & set(path(d,cd+c,k,j)) & set(path(d,da+ab+bc+c,k,j))
              if(len(S1)==0 or len(S2)==0 or len(S3)==0 or len(S4)==0): continue
              fin=False
              for a1 in S1:
                for a2 in S2:
                  for a3 in S3:
                    for a4 in S4:
                      if(compat(ab,a1,a2) and compat(bc+cd+da,a1,a2) and compat(ab+bc,a1,a3) and compat(da+cd,a1,a3) and compat(da,a1,a4) and compat(ab+bc+cd,a1,a4) and compat(bc,a2,a3) and compat(cd+da+ab,a2,a3)  and compat(bc+cd,a2,a4) and compat(da+ab,a2,a4) and compat(cd,a3,a4) and compat(da+ab+bc,a3,a4)): fin=True
                      if(fin): break
                    if(fin): break
                  if(fin): break
                if(fin): break
              if(not fin):
                #print("Not reducible: problem for  1234,",D[i],D[j],D[k],printDs(S1),printDs(S2),printDs(S3),printDs(S4))
                return False
  return True



# test if an S-handle S(a,b,c) is reducible
def testS(a,b,c):
	S1=[]; 
	for i in I:
		if compat(b+c,1,i): S1.append(path(c,b,1,i))
	
	for i in I:
		S2=[]
		S2.append(i)
		for j in S1:
			if not compatSet(a,S2,j): return False
	return True


# test if a H-handle H(a,b,c,d,e) is reducible
def testH(a,b,c,d,e):
	S1=[]; S2=[]
	for i in I:
		if compat(a+b,1,i): S1.append(path(a,b,1,i))
	for i in I:
		for j in I:
			if compat(d+e,i,j):
				S2.append(path(d,e,i,j))
	
	for i in S1:
		t=True
		for j in S2:
			if not compatSet(c,i,j): return False 
	return True


# test if a H-handle H(a,b,c,d,e) with an additional cycle on each side (of length f between a and b, and of length g between d and e) is reducible
def testH2C(a,b,c,d,e,f,g): 
	S1=[]; S2=[]
	for i in I:
		if compat(a+b,1,i) and compat(f,1,i): S1.append(path(a,b,1,i))
	for i in I:
		for j in I:
			if compat(d+e,i,j) and compat(g,i,j) and compat(3,1,j):
				S2.append(path(d,e,i,j))
	
	for i in S1:
		for j in S2:
			if not compatSet(c,i,j):  return False # print( [D[x] for x in i],[D[y] for y in j]);
	return True


# test if an S'-handle S'(a,b,c,d,e) is reducible (a is the biggest value, c the smallest ; d,e are the lengths of the paths going out of end-vertex of the path of length c
def testSprime(a,b,c,d,e): 
	S1=[]; S2=[]; 
	for cpt in range(0,5):
		i=X2[cpt]; 
		if compat(a+b,1,i): S1.append(path(b,a,1,i))
	for i in I:
		if compat(d+c+b,i,1):
			for j in I:
				if compat(d+e,i,j) and compat(e+c+b,j,1):
					F=filter(path(d,e,i,j),b+c)
					if len(F): S2.append(F)
	for i in S1:
		for j in S2:
			if not compatSet(c,i,j): return False
	return True


# test if an S''-handle S''(a,b,c,d,e,f,g) is reducible (a is the biggest value ; d,e are the lengths of the paths going out of end-vertex of the path of length b and f,g the lengths of the paths going out of c
def testSsecond(a,b,c,d,e,f,g):  
	
	return testHsecond(a,b,c,d,e,f,g,1,1) #True


# test if an H''-handle H''(a,b,c,d,e,f,g,h,i) is reducible. c is the length of the central path
def testHsecond(a,b,c,d,e,f,g,h,i): 
	SD=[]
	SG=Hgraph1Fixed(f,g,d,e,h,i)
	for si in I:
		for sj in I:
			if compat(a+b,si,sj):
				TT=path(a,b,si,sj)
				if len(TT): SD.append(TT)
	for k in SG:
		for l in SD:
			if not compatSet(c,k,l): return False
	return True

# some examples of tests. you can try other values, cf the paper
print("S(6,2,2) reducible: ",testS(6,2,2))
print("S(6,2,1) reducible: ",testS(6,2,1))
print("H(1,2,6,2,1) reducible: ",testH(1,2,6,2,1))
print("H(1,1,6,2,1) reducible: ",testH(1,1,6,2,1))
print("Cyclic handle on top left of Fig. 3 reducible: ",testCycle(2,4,1,1,2,1,2,1))
print("Cyclic handle with smaller length: ",testCycle(2,3,1,1,2,1,2,1))
print("S'(6,2,1,3,4) reducible: ",testSprime(6,2,1,3,4))
print("S'(6,2,1,2,3) reducible: ",testSprime(6,2,1,2,3))
print("S''(6,2,1,2,2,3,3) reducible: ",testSsecond(6,2,1,2,2,3,3))
print("S''(6,2,1,2,2,3,2) reducible: ",testSsecond(6,2,1,2,2,3,2))
print("H''(1,2,6,1,1,2,3,4,4) reducible: ",testHsecond(1,2,6,2,1,2,3,4,4))
print("H''(1,2,6,1,1,2,3,4,3) reducible: ",testHsecond(1,2,6,2,1,2,3,4,3))
