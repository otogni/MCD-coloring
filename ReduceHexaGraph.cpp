// Reduction of triangle free induced subgraph of the triangular lattice - new version with the graph of nodes and Sprime, Ssecond, Hsecond handle reductions
// parameters: p1 p2 p3 p4 p5 p6
#include <string> 
#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include <time.h>
using namespace std;


typedef struct pos{ int x; int y; int lg;} pos; // position of the vertices
const int N=200;
int verb=0;
int h=0,m,n,l,nn=0;
bool S[N][N];
int POS[N][N];
int  *NG[3], *LG[3];


static void echange(int *a, int *b) // exchanging the content of two adresses
{ 
	int tmp=*a;
	*a=*b;
	*b=tmp;
}

auto min (int a, int b, int c, int d) // return the min of 4 integers
{
	int x = (a<b) ? a : b;
	int y = (c<d) ? c : d;
	return (x<y) ? x : y;
}


int degree(int x, int y) // computes and returns the degree of a vertex
{
	int cpt=0;
	if(y<h-1 && S[x][y+1]) cpt++;
	if(x<l-1 && S[x+1][y]) cpt++;
	if(x<l-1 && y<h-1 && S[x+1][y+1]) cpt++;
	if(y>0 && S[x][y-1]) cpt++;
	if(x>0 && S[x-1][y]) cpt++;
	if(x>0 && y>0 && S[x-1][y-1]) cpt++;

	return cpt;
}

bool NotTriangle(int x, int y) // return True if there is no triangle in the graph
{	int cpt=0;
	if(degree(x,y)<2) return true;
	if(degree(x,y)>3) return false;
	if(x<l-1 && y<h-1 && (S[x+1][y] && S[x+1][y+1])) cpt++;
	if(x<l-1 && y<h-1 && (S[x+1][y+1] && S[x][y+1])) cpt++;
	if(x>0 && y<h-1 && (S[x][y+1] && S[x-1][y])) cpt++;
	if(x>0 && y>0 && (S[x-1][y] && S[x-1][y-1])) cpt++;
	if(x>0 && y>0  && (S[x-1][y-1] && S[x][y-1])) cpt++;
	if(x<l-1 && y>0 && (S[x][y-1] && S[x+1][y])) cpt++;
	if(cpt>0) return false;
	return true;
}



void flower(int i, int j) // put a flower (consisting of a central 6-cycle and 6 9-cycles around, see Fig. 1 of the paper) at coordinate (i,j)
{
  S[i][j+1]=S[i][j+2]=S[i][j+3]=true;
  S[i+1][j+1]=S[i+1][j]=S[i+1][j+4]=S[i+1][j+5]=true;
  S[i+2][j]=S[i+2][j+2]=S[i+2][j+4]=S[i+2][j+6]=true;
  S[i+3][j]=S[i+3][j+3]=S[i+3][j+4]=S[i+3][j+7]=true;
  S[i+4][j+1]=S[i+4][j+2]=S[i+4][j+3]=S[i+4][j+5]=S[i+4][j+6]=S[i+4][j+7]=true;
  S[i+5][j+1]=S[i+5][j+4]=S[i+5][j+5]=S[i+5][j+8]=true;
  S[i+6][j+2]=S[i+6][j+4]=S[i+6][j+6]=S[i+6][j+8]=true;
  S[i+7][j+3]=S[i+7][j+4]=S[i+7][j+7]=S[i+7][j+8]=true;
  S[i+8][j+5]=S[i+8][j+6]=S[i+8][j+7]=true;

}



int genereGraph(int lg, int aleat) // generate an hexagonal graph semi-randomly by putting flowers and adding random vertices and return the number of vertices
{
  int Nb=0, cpt=0;
  bool res;
  
  for(int i=0; i<h/15; i++)
   { 
    for(int j=0; j<l/15 ;j++)
	  
	   flower(5+i*lg+(rand()%aleat),5+j*lg+(rand()%aleat)); 
   }
  for(int i=0; i<5*l*h ;i++)
    {
       int x=rand()%l, y=rand()%h;
       if(!S[x][y] && NotTriangle(x,y)) {S[x][y]=true; } 
       if(S[x][y] && !NotTriangle(x,y)) {cerr << "Warning, triangle detected!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl; exit(5);}
    }

   for(int i=0; i<l ;i++)
    for(int j=0;j<h; j++)
      if(!S[i][j] && NotTriangle(i,j)) {S[i][j]=true; } 

  for(int i=0; i<l ;i++)
    for(int j=0;j<h; j++)
      if(S[i][j]) Nb++;
   return Nb;
}


int supprDeg1Nodes() // remove degree 1 vertices iteratively
{ 
	int nb=0;
	bool change=true;
	while(change)
	{  change=false;
	   for(int i=0; i<l ;i++)
	    for(int j=0;j<h; j++)
             if(S[i][j] && degree(i,j)<=1) {S[i][j]=false; nb++; change=true;}
	}
	return nb;
}


pos otherSide(int x, int y, int o) // return the position of the other side of the path starting at (x,y) in direction o
{
	int vd=o, cpt=1;
	pos p;
	if(vd<2) x++; if(vd==1 || vd==2) y++; if(vd==3 || vd==4) x--; if(vd==4 || vd==5) y--;
	while(degree(x,y)==2)
	  {     
		if(vd!=3 && x<l-1 && S[x+1][y]) {cpt++; x++; vd=0;}
		else if(vd!=4 && x<l-1 && y< h-1 && S[x+1][y+1]) {cpt++; x++; y++; vd=1;}
		else if(vd!=5 && y< h-1 && S[x][y+1]) {cpt++; y++; vd=2;}
		else if(vd!=0 && x>0 && S[x-1][y]) {cpt++; x--; vd=3;}
		else if(vd!=1 && x>0 && y>0 && S[x-1][y-1]) {cpt++; x--; y--; vd=4;}
		else if(vd!=2 && y>0 && S[x][y-1]) {cpt++; y--; vd=5;}
		else cerr << "Problem otherSide" << endl;
	  } 
	p.x=x; p.y=y; p.lg=cpt;
	return p;
}


bool addNeighbor(int i, int j, int x, int y, int lg) // add to the node at position (i,j) the neighbor at position (x,y) with an edge of length lg
{
	int node=POS[i][j]; int p=POS[x][y];
	if(p==-1) { p=nn; POS[x][y]=p; nn++;} 
	int k=0;
	if(LG[0][node]>0) k++;
	if(LG[1][node]>0) k++;
	if(LG[2][node]>0) cerr << "Node (" << i << "," << j << ") has more than 3 neighbors !!" << endl;
	NG[k][node]=p; LG[k][node]=lg;
	return true;
}


int convertToNodegraph() // covert the S[i][j] matrix to a 3-regular graph of the nodes (NG) and lengths (LG) 
{
	pos p;
	nn=0;
	for(int i=0; i<l ;i++)
    	    for(int j=0;j<h; j++)
      		if(S[i][j] && degree(i,j)==3) 
		   { if(POS[i][j]==-1) {POS[i][j]=nn++;} 
		     if(S[i+1][j]) {p=otherSide(i,j,0);  addNeighbor(i,j,p.x,p.y,p.lg);}
		     if(S[i+1][j+1]) {p=otherSide(i,j,1);  addNeighbor(i,j,p.x,p.y,p.lg);}
		     if(S[i][j+1]) {p=otherSide(i,j,2);  addNeighbor(i,j,p.x,p.y,p.lg);}
		     if(S[i-1][j]) {p=otherSide(i,j,3);  addNeighbor(i,j,p.x,p.y,p.lg);}
		     if(S[i-1][j-1]) {p=otherSide(i,j,4);  addNeighbor(i,j,p.x,p.y,p.lg);}
		     if(S[i][j-1]) {p=otherSide(i,j,5);  addNeighbor(i,j,p.x,p.y,p.lg);}
		   }
	return 1;
}


void printNeighbors() // print the list of neighbors and lengths of each vertex
{
     for(int i=0; i<nn;i++)
	if(NG[0][i]>-1) 
	 cout << "node " << i << "-> neighbors : " << NG[0][i] << "," << NG[1][i] << "," << NG[2][i] << ", length :" << LG[0][i] << "," << LG[1][i] << "," << LG[2][i] << endl;
	
}


bool isGood() // test if NG is symetric
{
	for(int i=0; i<nn;i++)
	  if(NG[0][i]!=-1) 
	    for(int j=0;j<3;j++)
	      { int x=NG[j][i];
	      if(NG[0][x]!=i && NG[1][x]!=i && NG[2][x]!=i) {cerr << "Symmetry problem for " << i << " and " << x << endl; printNeighbors(); exit(3);}
	      }
	return true;
}



void node2Files(int num) // from the graph of nodes, create the file NR-num.txt with the adjacencies and lengths and the file NR-num.pos with the position of each node. This allow to plot the graph using gnuplot
{
  ofstream fic,fic2; 
  char nomFic[30]; sprintf(nomFic,"NR-%d.txt",num);
  fic.open(nomFic,ios::out);
   fic << "#Graphes réduit : " << endl;
   for(int i=0; i<nn;i++)
	if(NG[0][i]>-1) {
		if(NG[0][i]>i) fic << i << "\t" << NG[0][i] << "\t" << LG[0][i] << endl;
		if(NG[1][i]>i) fic << i << "\t" << NG[1][i] << "\t" << LG[1][i] << endl;
		if(NG[2][i]>i) fic << i << "\t" << NG[2][i] << "\t" << LG[2][i] << endl;
	}
  bool deb=true;
  sprintf(nomFic,"NR-%d.pos",num);
  fic2.open(nomFic,ios::out);
  for(int i=0;i<l;i++)
    for(int j=0;j<h;j++)
      if(S[i][j] && POS[i][j]!=-1 && NG[0][POS[i][j]]>-1)
        if(deb) { fic2 << "'" << POS[i][j] << "': [" << i << "," << j << "]"; deb=false;}
        else fic2 << ", '" << POS[i][j] << "': [" << i << "," << j << "]";
  fic2 << endl;
  fic.close();
  fic2.close();
}


int idNeigh(int nd, int id) // return the id of the edge that links my neighbor vd to me (nd) by id
{
	int lg=LG[id][nd], vd=NG[id][nd]; if(vd<0 || lg<1) cerr << nd << "Pb na pas de voisin sur " << id << endl;
	int i=0;
	while(i<3 && (NG[i][vd] != nd || LG[i][vd] != lg)) i++;
	if(i==3) {cerr << "Pb dysimetrie !!" << nd << "," << vd << isGood() << endl; printNeighbors();exit(1);}
	return i;
}


int idNext(int nd, int id1, int id2) // return the third index of nd different from nd1 and nd2
{	
	int i=0;
	while(i==id1 || i==id2 && i<3) i++;
	if(i==3) cerr << "Pb idNext !" << endl;
	return i; 
}



void reconnect(int nd1, int nd2, int nd3, int lg1, int lg2) // update the neighbors and lengths when suppressing node nd2 between nd1 and nd3 
{  
	int i=0;
	while(NG[i][nd1]!=nd2 || LG[i][nd1]!=lg1) i++;
	if(i>2) cerr << "Problem reconnect !" << endl; 
	NG[i][nd1]=nd3;
	LG[i][nd1]=lg1+lg2;
	
	if(nd1==nd3)
	{
	  i=0;
	  while(NG[i][nd1]!=nd2 || LG[i][nd1]!=lg2) i++;
	  if(i>2) cerr << "Problem reconnect !" << endl; 
	  NG[i][nd1]=nd3;
	  LG[i][nd1]=lg1+lg2;
	}
}



void reconnectVois(int nd, int id) // nd will be removed, so we have to reconnect the neighbor of nd in direction id
{
	int vd=NG[id][nd]; int id1=0; 
	while(NG[id1][vd]!=nd) id1++;
	int vd1=NG[(id1+1)%3][vd], lg1=LG[(id1+1)%3][vd]; 
	int vd2=NG[(id1+2)%3][vd], lg2=LG[(id1+2)%3][vd]; 
	reconnect(vd1,vd,vd2,lg1,lg2);
	if(vd1!=vd2) reconnect(vd2,vd,vd1,lg2,lg1);
}



bool supprPath(int nd, int id)	// suppression of the path going out of nd in diretion id
{
	bool loopn=false,loopv=false;
	int nd1,nd2,vd,vd1,vd2;
	int lgn1,lgn2,lgv1,lgv2;
	vd=NG[id][nd]; 
	int i=(id + 1) % 3;
	nd1=NG[i][nd]; lgn1=LG[i][nd];
	i=(i + 1) % 3;
	nd2=NG[i][nd];lgn2=LG[i][nd]; 
	if(nd1==nd2) loopn=true;
	
	i=0;
	while(NG[i][vd]!=nd || LG[i][vd]!=LG[id][nd]) i++; if(i==3) { cerr << "Pb index depasse !!" << nd << "," << vd << endl; exit(6);}
	vd1=NG[(i+1)%3][vd]; vd2=NG[(i+2)%3][vd]; lgv1=LG[(i+1)%3][vd]; lgv2=LG[(i+2)%3][vd];
	if(vd1==vd2) loopv=true;

	if(nd1==vd && nd2==vd) {NG[0][nd]=-1; NG[0][vd]=-1; return true;} 
	if(nd1==vd || nd2==vd)
	  { 
	    if(nd2==vd) {echange(&nd1,&nd2); echange(&lgn1,&lgn2);}
	    if(vd2==nd) {echange(&vd1,&vd2); echange(&lgv1,&lgv2);}
	    if(nd1==vd && vd1==nd) 
		{ reconnect(nd2,nd,vd,lgn2,lgn1); reconnect(nd2,vd,vd2,lgn2+lgn1,lgv2); 
		  if(nd2!=vd2)
		    { reconnect(vd2,vd,nd,lgv2,lgv1); reconnect(vd2,nd,nd2,lgv2+lgv1,lgn2);}
		  NG[0][nd]=NG[1][nd]=NG[2][nd]=-1; NG[0][vd]=NG[1][vd]=NG[2][vd]=-1; 
		  return true;
	        }
	    else cerr << "Big problem !!" << endl;
	  }
	
	if(nd1!=nd2) { reconnect(nd1,nd,nd2,lgn1,lgn2); reconnect(nd2,nd,nd1,lgn2,lgn1);}
	else reconnect(nd1,nd,nd1,lgn1,lgn2); 
	if(vd1!=vd2) { reconnect(vd1,vd,vd2,lgv1,lgv2); reconnect(vd2,vd,vd1,lgv2,lgv1);}
	else reconnect(vd1,vd,vd1,lgv1,lgv2);
	
	NG[0][nd]=NG[1][nd]=NG[2][nd]=-1; NG[0][vd]=NG[1][vd]=NG[2][vd]=-1; 
	return true;
	
}


bool Phandle(int nd)	// P-handle configuration
{	 
	if(LG[0][nd]>7) { supprPath(nd,0); if (verb) cout << "Path(" << LG[0][nd] << ") depuis " << nd << endl; return true;}
	else if(LG[1][nd]>7) {supprPath(nd,1); if(verb) cout << "Path(" << LG[1][nd] << ") depuis " << nd << endl; return true;}
	else if(LG[2][nd]>7) {supprPath(nd,2); if(verb) cout << "Path(" << LG[2][nd] << ") depuis " << nd << endl; return true;}
	return false;
}


bool otherPath(int nd, int id, int dst, int lg) // search for a second path of the same parity than the one starting at nd in direction id and going to dst
{
	int nd1=NG[(id+1)%3][nd], lg1=LG[(id+1)%3][nd];
	int nd2=NG[(id+2)%3][nd], lg2=LG[(id+2)%3][nd];

	if(nd1==dst) return (lg>= lg1 && lg%2==lg1%2);
	if(nd2==dst) return (lg>= lg2 && lg%2==lg2%2);
	if(lg1>=lg && lg2>=lg) return false;
	if(nd1==nd2) 
	  return otherPath(nd1,idNext(nd1,idNeigh(nd,(id+1)%3), idNeigh(nd,(id+2)%3)),dst,lg-lg1);
	if(lg1<lg && lg2<lg)
	  return (otherPath(nd1,idNeigh(nd,(id+1)%3),dst,lg-lg1) || otherPath(nd2,idNeigh(nd,(id+2)%3),dst,lg-lg2));
	else if(lg1<lg) return otherPath(nd1,idNeigh(nd,(id+1)%3),dst,lg-lg1);
	else return otherPath(nd2,idNeigh(nd,(id+2)%3),dst,lg-lg2);
}



bool PathOtherId(int nd, int id, int dst, int lg) // return true if there exists a path of length lg between nd and dst not in direction id
{
	int nd1=NG[(id+1)%3][nd], lg1=LG[(id+1)%3][nd];
	int nd2=NG[(id+2)%3][nd], lg2=LG[(id+2)%3][nd];

	if(nd1==dst) return (lg == lg1);
	if(nd2==dst) return (lg == lg2);
	if(lg1>=lg && lg2>=lg) return false;
	if(nd1==nd2) 
	  return PathOtherId(nd1,idNext(nd1,idNeigh(nd,(id+1)%3), idNeigh(nd,(id+2)%3)),dst,lg-lg1);
	if(lg1<lg && lg2<lg)
	  return (PathOtherId(nd1,idNeigh(nd,(id+1)%3),dst,lg-lg1) || PathOtherId(nd2,idNeigh(nd,(id+2)%3),dst,lg-lg2));
	else if(lg1<lg) return PathOtherId(nd1,idNeigh(nd,(id+1)%3),dst,lg-lg1);
	else return PathOtherId(nd2,idNeigh(nd,(id+2)%3),dst,lg-lg2);
}


bool PathId(int nd, int id, int dst, int lg) // return true if there exists a path of length lg between nd and dst in direction id
{
	int nd1=NG[id][nd], lg1=LG[id][nd];

	if(nd1==dst) return (lg == lg1);
	if(lg1>=lg) return false;
	return PathOtherId(nd1,idNeigh(nd,id),dst,lg-lg1);
	
}

int idPath(int nd, int id, int dst, int lg) // return the id of the first link of the path of length lg between nd and dst not in direction id ; -1 if no such path
{
   if(PathId(nd,(id+1)%3,dst,lg)) return id+1%3; 
   else if (PathId(nd,(id+2)%3,dst,lg)) return id+2%3;
   else return -1;
}


int LengthThirdPath(int nd, int id1, int id2) // returns the length of the third path from nd (neither in direction nd1 nor nd2)
{
	int i=0;
	while (i==id1 || i== id2) i++;
	return(LG[i][nd]);
}


bool PPhandle(int nd) // test the presence of a parity handle PP-handle from nd
{
	if(LG[0][nd] >2 && NG[0][nd]!=nd && otherPath(nd,0,NG[0][nd],LG[0][nd])) { if(verb) cout << "Parity path(" << LG[0][nd] << ") à partir de " << nd << " direction " << NG[0][nd] << endl; supprPath(nd,0); return true;}
	else if(LG[1][nd] >2 && NG[1][nd]!=nd && NG[1][nd]!=nd && otherPath(nd,1,NG[1][nd],LG[1][nd])) { if(verb) cout << "Parity path(" << LG[1][nd] << ") à partir de " << nd << " direction " << NG[1][nd] << endl; supprPath(nd,1);  return true;}
	else if(LG[2][nd] >2 && NG[2][nd]!=nd && otherPath(nd,2,NG[2][nd],LG[2][nd])) { if(verb) cout << "Parity path(" << LG[2][nd] << ") à partir de " << nd << " direction " << NG[2][nd] << endl; supprPath(nd,2); return true;}
	return false;
}


bool Shandle(int nd, bool sprime, bool ssecond)	// test the presence of a Star handle (S-handle) or S'-handle or S''-handle ; sprime=True for testing S'-handles ; S''=True for testing S''-handles
{	
	int d1,d2,d3,d4,d5,d6,d7,im=0, imc=2;
	bool f=false;
	d1=LG[0][nd]; d2=LG[1][nd]; d3=LG[2][nd];
	if(d1<d2) {echange(&d1,&d2); im=1;}
	if(d2<d3) {echange(&d2,&d3); if(im==0) imc=1; else imc=0;}
	if(d1<d2) {echange(&d1,&d2); im=2;}
	if(d1<=2) return false;
	if(d1==7 && d2>=2) {supprPath(nd,im); f=true;}
	if(d1==6 && d2>=2 && d3>=2) {supprPath(nd,im); f=true;}
	if(d1==5 && d2>=3 && d3>=3) {supprPath(nd,im); f=true;}
	if(d1==4 && d2>=4 && d3>=4) {supprPath(nd,im); f=true;}
	if(f) {if(verb) cout << "S(" << d1 << "," << d2 << "," << d3 << ") from " << nd << endl; return true;}
	
	if(sprime)
	{  
	  int nd2=NG[imc][nd]; 
	  int i=0; 
	  while(i<3 && (NG[i][nd2]!=nd || LG[i][nd2]!=d3)) i++;
	  if(i==3) {cerr << "Problem Shandle " << nd2 << " is not a neighbor of " << nd << endl; exit(1);}
	  d4=LG[(i+1)%3][nd2];
	  d5=LG[(i+2)%3][nd2];	
	  if(d1==7 && d4>2 && d5>2 && d4+d5>=9) { f=true;} // S'(7,1,1,3,6) ou S''(7,1,1,4,5)
	  if(d1==6 && d2>=3 && d4>1 && d5>1 && (d4>+d5>5)) { f=true;} // S'(6,3,1,3,3) ou S'(6,3,1,2,4)
	  if(d1==6 && d2==2 && d4>1 && d5>1) { f=true;} // S'(6,2,1,2,2)
	  if(d1==5 && d2>=3 && d3>=2 && d4>1 && d5>1) { f=true;} // S'(5,3,2,2,2)
	  else if(d1==5 && d2>=4 && d3>=2 && d4>1 && d5>1 && d4+d5>3) { f=true;} // S'(5,4,2,2,2)
	  else if(d1==5 && d2>=4 && d4>1 && d5>1 && d4+d5>=6) { f=true;} // S'(5,4,1,3,3) ou S'(5,4,1,2,4)
	  else if(d1==5 && d2>=3 && d4>3 && d5>3) { f=true;} // S'(5,3,1,4,4)
	  if(d1==4 && d2>=4 && d4>2 && d5>2 && d4+d5>=7) { f=true;} // S'(4,4,1,3,4)
	  else if(d1==4 && d2>=4 && d3>1 && d4>1 && d5>1 && d4+d5>=5) {f=true;} // S'(4,4,2,2,3)
	  else if(d1==4 && d2>=4 && d3>2 && d4>1 && d5>1) { f=true;} // S'(4,4,3,2,2)
	  else if(d1==4 && d2>=4 && d3>2 && d4+d5>4) { f=true;} // S'(4,4,3,1,4)
	  if(f) { if(verb) cout << "S'(" << d1 << "," << d2 << "," << d3 << "," << d4 << "," << d5 <<") from " << nd << endl; supprPath(nd,im); return true;}
	  if(ssecond)
	  { 
	    i=0; 
	    while(i<3 && (i==imc || i==im)) i++;
	    int nd3=NG[i][nd];
	    i=0; 
	    while(i<3 && (NG[i][nd3]!=nd || LG[i][nd3]!=d2)) i++;
	    if(i==3) {cerr << "Problem Sshandle " << nd3 << " is not a neighbor of " << nd << endl; node2Files(0);exit(1);}
	    d6=LG[(i+1)%3][nd3];
	    d7=LG[(i+2)%3][nd3];	 
	    if(d1==7 && min(d4,d5,d6,d7)>2 && d4+d5+d6+d7>=14) { f=true;} // S''(7,1,1,3,4,3,4) ou S''(7,1,1,3,3,4,4) ou S''(7,1,1,3,3,3,5)
	    if(d1==6 && d2>=2 && d4>2 && d5>2 && d6>1 && d7>1) { f=true;} // S''(6,2,1,2,2,3,3)
	    else if(d1==6 && d2>=2 && d4>1 && d5>1 && d4+d5>=5 && d6>2 && d7>2) { f=true;} // S''(6,2,1,3,3,2,3)
	    else if(d1==6 && d4>1 && d5>1 && d6>1 && d7>1 && d4+d5+d6+d7>=14) { f=true;} // S''(6,1,1,3,3,4,4) ou S''(6,1,1,2,5,2,5)
	    else if(d1==6 && d4>1 && d5>1 && d6>1 && d7>1 && d4+d6>=5 && d4+d7>=5 && d5+d6>=5 && d5+d7>=5 && d4+d5>=6 && d6+d7>=6 && d4+d5+d6+d7>=13) { f=true;} // S''(6,1,1,2,4,3,4)
	    if(d1==5 && d2>=3 && d3>=2 && d4>1 && d5>1 && d6>1 && d7>1) { f=true;} // S''(5,3,2,2,2,2,2)
	    else if(d1==5 && d2>=2 && d3>=2 && d4>1 && d5>1 && d6>1 && d7>1 && d4+d5+d6+d7>=10) { f=true;} // S''(5,2,2,2,3,2,3) ou S''(5,2,2,3,3,2,2) ou S''(5,2,2,2,4,2,2)
	    else if(d1==5 && d2>=2 && ((d4>2 && d5>2 && d6>2 && d7>2) || (d4>2 && d5>2 && d4+d5>6 && d6>1 && d7>1 && d6+d7>4))) { f=true;} // S''(5,2,1,3,3,3,3) ou S''(5,2,1,2,3,3,4)
	    else if(d1==5 && d4>2 && d5>2 && d6>2 && d7>2 && d4+d5+d6+d7>=14) { f=true;} // S''(5,1,1,3,3,4,4) ou S''(5,1,1,3,4,3,4)
	    if(d1==4 && d2>=4 && d3>1 && d4>1 && d5>1 && d4+d5>=3) {f=true;} // S''(4,4,2,1,2,2,2) 
	    else if(d1==4 && d2>=4 && d4>2 && d5>3 && d4>1 && d5>1) {f=true;} // S''(4,4,1,2,2,3,3) 
	    else if(d1==4 && d2>=4 && d4>2 && d5>3 && d6+d7>=5) {f=true;} // S''(4,4,1,1,4,3,3) 
	    else if(d1==4 && d2>=3 && d3>=3 && d4>1 && d5>1 && d6+d7>=3) {f=true;} // S''(4,3,3,1,2,2,2) 
	    else if(d1==4 && d2>=3 && d3>=2 && d4>2 && d5>2 && d6+d7>=3) {f=true;} // S''(4,3,2,1,2,3,3) 
	    else if(d1==4 && d2>=3 && d4>2 && d5>2 && d4+d5>=7 && d6>1 && d7>1) {f=true;} // S''(4,3,1,2,2,3,4)
	    else if(d1==4 && d2>=2 && d3>=2 && d4>1 && d5>1 && d4+d5>=5 && d6>1 && d7>1 && d6+d7>=5) {f=true;} // S''(4,2,2,2,3,2,3)
	    if(d1==3 && d2==3 && d3==3 && d4>1 && d5>1 && d4+d5>=5 && d6>1 && d7>1 && d6+d7>=5) {f=true;} // S''(3,3,3,2,3,2,3)
	    
	    if(f) {supprPath(nd,im); if(verb) cout << "S''(" << d1 << "," << d2 << "," << d3 << "," << d4 << "," << d5 << "," << d6 << "," << d7 << ") from " << nd << endl; return true;} 
	  }
	}	
	return false;
}


bool testChandle(int nd,int nd2,int id2,int id3, int id4, int id5, int d1, int d2, int d3, int d4, int d5) // test for H-handles with cycles (Fig. 5 of the paper). It is a subfunction of Hhandle
{
	bool f=false; int l=-1;
	int nd4=NG[id4][nd2], nd5=NG[id5][nd2], ndd2=NG[id2][nd], ndd3=NG[id3][nd];
	 if(d1==2)
	{  
	   if(d3==1 && d2>=2 && d4>=4 && d5==1 && PathOtherId(ndd3,idNeigh(nd,id3),nd5,2)) f=true; // H(1,2,2,4,1)+P2(v3v5)
	   else if(d3==2 && d2>=3 && d4>=3 && d5==2 && (PathOtherId(ndd3,idNeigh(nd,id3),nd5,2) || PathOtherId(ndd3,idNeigh(nd,id3),nd5,4)|| PathOtherId(ndd3,idNeigh(nd,id3),nd5,6))) f=true; // H(2,3,2,3,2)+Px(v3v5), x=2,4 or 6
	   else if(d3==2 && d2==2 && d4>=4 && d5==2 && (PathOtherId(ndd2,idNeigh(nd,id2),nd5,2) || PathOtherId(ndd3,idNeigh(nd,id3),nd5,2))) f=true; // H(2,2,2,4,2)+P2(v3v5)
	   else if(d3==2 && d2==2 && d4>=4 && d5==2 && (((l=idPath(nd5,idNeigh(nd2,id5),ndd2,4))>-1 && LengthThirdPath(nd5,l,idNeigh(nd2,id5))>=2) || ((l=idPath(nd5,idNeigh(nd2,id5),ndd2,6))>-1 && LengthThirdPath(nd5,l,idNeigh(nd2,id5))>=2))) f=true; // H(2,2,2,4,2)+P2(v5+) + Px(v2v5), x=4 ou 6
	   else if(d3==2 && d2>=2 && d4>=4 && d5==2 && (((l=idPath(nd5,idNeigh(nd2,id5),ndd3,4))>-1 && LengthThirdPath(nd5,l,idNeigh(nd2,id5))>=2) || ((l=idPath(nd5,idNeigh(nd2,id5),ndd3,6))>-1 && LengthThirdPath(nd5,l,idNeigh(nd2,id5))>=2))) f=true; // H(2,2,2,4,2)+P2(v5+) + Px(v3v5), x=4 ou 6
	   else if(d3==1 && d2==5 && d4==2 && d5==2 && ((l=idPath(ndd3,idNeigh(nd,id3),nd4,1))>-1 && LengthThirdPath(ndd3,l,idNeigh(nd,id3))>=2 && (l=idPath(nd4,idNeigh(nd2,id4),ndd3,1))>-1 && LengthThirdPath(nd4,l,idNeigh(nd2,id4))>=2)) f=true; //H(1,5,2,2,2)+P1(v3v4)+P2(v3+)+P2(v4+)
	   else if(d3==1 && d2==5 && d4>=2 && d5==2 && ((l=idPath(ndd3,idNeigh(nd,id3),nd5,1))>-1 && LengthThirdPath(ndd3,l,idNeigh(nd,id3))>=2 && (l=idPath(nd5,idNeigh(nd2,id4),ndd3,1))>-1 && LengthThirdPath(nd5,l,idNeigh(nd2,id5))>=2)) f=true; //H(1,5,2,2,2)+P1(v3v5)+P2(v3+)+P2(v5+)
	}
	else if(d1==3)
	{ 
	   if(d3==1 && d2>=3 && d4>=4 && d5==1 && ((l=idPath(ndd3,idNeigh(nd,id3),nd5,3))>-1 && LengthThirdPath(ndd3,l,idNeigh(nd,id3))>=2 && (l=idPath(nd5,idNeigh(nd2,id5),ndd3,3))>-1 && LengthThirdPath(nd5,l,idNeigh(nd2,id5))>=2)) {f=true;} // H(1,3,3,4,1)+P3(v3v5) + P2(v5+) + P2(v3+)
	   else if(d3==2 && d2==2 && d4==2 && d5==2 && PathOtherId(ndd2,idNeigh(nd,id2),ndd3,2) && PathOtherId(nd4,idNeigh(nd2,id4),nd5,2) && ((PathOtherId(ndd2,idNeigh(nd,id2),nd4,3) || PathOtherId(ndd3,idNeigh(nd,id3),nd4,3) || PathOtherId(ndd2,idNeigh(nd,id2),nd5,3) || PathOtherId(ndd3,idNeigh(nd,id3),nd5,3)))) f=true; // H(2,2,3,2,2)+P2(v2v3)+P2(v4v5)+P3(v2v4)
	   else if(d3==1 && d2>=2 && d4==5 && d5==1 && (l=idPath(nd5,idNeigh(nd2,id5),ndd3,3))>-1 && LengthThirdPath(nd5,l,idNeigh(nd2,id5))>=2) {f=true;} // H(1,2,3,5,1) + P3(v3v5) + P2(v5+)
	   
	   else if(d3==1 && d2>=3 && d4>=4 && d5==2 && (l=idPath(nd5,idNeigh(nd2,id5),ndd3,3))>-1 && LengthThirdPath(nd5,l,idNeigh(nd2,id5))>=2) {f=true;} // H(1,3,3,4,2)+P3(v3v5) + P2(v5+)
	   else if(d3==1 && d2>=2 && d4==4 && d5==3 && PathOtherId(ndd3,idNeigh(nd,id3),nd5,2)) f=true; //H(1,2,3,4,3)+P2(v3v5)
	}
	else  if(d1==4)
	{
	   if(d3==2 && d2==2 && d4==2 && d5==2){
	     if(PathOtherId(ndd2,idNeigh(nd,id2),ndd3,2) && PathOtherId(nd4,idNeigh(nd2,id4),nd5,2)) f=true;// H(2,2,4,2,2)+P2(v2v3)+P2(v4v5)
	     else if(PathOtherId(ndd2,idNeigh(nd,id2),nd4,2) || PathOtherId(ndd2,idNeigh(nd,id2),nd5,2) || PathOtherId(ndd3,idNeigh(nd,id3),nd4,2) || PathOtherId(ndd3,idNeigh(nd,id3),nd5,2)) f=true; // H(2,2,4,2,2)+P2(v2v4)
	   }
	   else if(d3==1 && d2>=2 && d4>=2 && d5==1 && PathOtherId(ndd3,idNeigh(nd,id3),nd5,4)) f=true; // H(1,2,4,2,1)+P4(v3v5) 
	   else if(d3==1 && d2==2 && d4==2 && d5==2 && (PathOtherId(ndd2,idNeigh(nd,id2),nd5,2) || PathOtherId(ndd2,idNeigh(nd,id2),nd4,2)) && PathOtherId(nd4,idNeigh(nd2,id4),nd5,2)) f=true; // H(1,2,4,2,2)+P2(v2v5) + P2(v4v5)
	   else if(d3==1 && d2==2 && d4>=3 && d5==2 && (PathOtherId(ndd2,idNeigh(nd,id2),nd5,2) || PathOtherId(ndd2,idNeigh(nd,id2),nd5,4) || PathOtherId(ndd2,idNeigh(nd,id2),nd5,5))) f=true; // H(1,2,4,3,2)+Px(v2v5), x=2,4 or 5
	   else if(d3==1 && d2==2 && d4>=4 && d5==2 && (
	   PathOtherId(ndd2,idNeigh(nd,id2),nd5,3) || PathOtherId(ndd2,idNeigh(nd,id2),nd5,4))) f=true; // H(1,2,4,4,2) + P3(v2v5) or P4(v2v5)
	   else if(d3==1 && d2>=3 && d4==3 && d5==3 && (PathOtherId(nd4,idNeigh(nd2,id4),nd5,3) || PathOtherId(nd4,idNeigh(nd2,id4),nd5,5))) f=true; // H(1,3,4,3,3)+P3(v4v5) or P5(v4v5)
	   else if(d3==1 && d2==3 && d4>=3 && d5==2 && nd5==ndd2) f=true; // H(1,3,4,3,2)+v2==v5
	}
	else if(d1==5 && d3==1){
	   if(d2==2 && d4>=2 && d5==1 && PathOtherId(ndd2,idNeigh(nd,id2),nd5,7)) f=true; // H(1,2,5,2,1)+P7(v2v5)
	   else if(d2==2 && d4==2 && d5==2 && PathOtherId(nd4,idNeigh(nd2,id4),nd5,2) && (PathOtherId(ndd2,idNeigh(nd,id2),nd5,3) || PathOtherId(ndd2,idNeigh(nd,id2),nd4,3))) f=true; // H(1,2,5,2,2)+P3(v2v4)+P2(v4v5)
	   else if(d2==2 && d4>=3 && d5==1 && PathOtherId(ndd2,idNeigh(nd,id2),nd5,5)) f=true; // H(1,2,5,3,1)+P5(v2v5)
	   else if(d2>=2 && d4>=4 && d5==1 && PathOtherId(ndd3,idNeigh(nd,id3),nd5,2)) f=true; // H(1,2,5,4,1)+P2(v3v5)
	}
	else if(d1==6 && d3==1 && d2==1 && d4==2 && d5==1 && (nd4==ndd2 || nd4==ndd3)) f=true; // H(1,1,6,2,1)+v1==v4
	else if(d1==6 && d3==1 && d2==1 && d4>=2 && d5==1 && (PathOtherId(ndd3,idNeigh(nd,id3),nd5,4) || PathOtherId(ndd3,idNeigh(nd,id3),nd5,6) || PathOtherId(ndd2,idNeigh(nd,id2),nd5,4) || PathOtherId(ndd2,idNeigh(nd,id2),nd5,6))) f=true; // H(1,1,6,2,1)+P6(v3v5) ou P4(v3v5)

	else if(d1==7 && d3==1 && d2==1 && d4==1 && d5==1 && (PathOtherId(ndd2,idNeigh(nd,id2),nd4,4) || PathOtherId(ndd3,idNeigh(nd,id3),nd4,4) || PathOtherId(ndd2,idNeigh(nd,id2),nd5,4) || PathOtherId(ndd3,idNeigh(nd,id3),nd5,4))) f=true; // H(1,1,7,1,1)+ P4(v3v5)
	else if(d1==7 && d3==1 && d2==1 && d4==1 && d5==1 && (PathOtherId(ndd2,idNeigh(nd,id2),nd4,6) || PathOtherId(ndd3,idNeigh(nd,id3),nd4,6) || PathOtherId(ndd2,idNeigh(nd,id2),nd5,6) || PathOtherId(ndd3,idNeigh(nd,id3),nd5,6))) f=true; // H(1,1,7,1,1)+ P6(v3v5)
	if(f) { if(verb) cout << "Hhandle+C (" << d3 << "," << d2 << "," << d1 << "," << d4 << "," << d5 << ") depuis " << nd << endl; return true;}
	return false;
}



bool Hhandle(int nd, int id, bool hcycle, bool hsecond)	// test for Hhandles et H''-handles. d1=length of central path; d2>=d3 on one sideand d4>=d5 on the other; hcycle=True for testing cyclic handles and hsecond=True for testing H''-handles
{ 
	int cp=0,d1,d2,d3,d4,d5,d6,d7,d8,d9,id2,id3,id4,id5;
	bool f=false;
	if(NG[0][nd]<0) cerr << "Problem Hhandle !!" << endl;
	d1=LG[id][nd]; id2=(id+1)%3; id3=(id+2)%3;
	d2=LG[id2][nd];
	d3=LG[id3][nd]; 
	//if(d1<=2) return false;
	if(d2<d3) {echange(&d2,&d3); id2=id3; id3=(id+1)%3;}
	
	int nd2=NG[id][nd]; 
	int i=0; 
	while(i<3 && (NG[i][nd2]!=nd || LG[i][nd2]!=d1)) i++;
	if(i==3) {cerr << "Pb voisins Hhandle " << nd2 << " nest pas voisin de " << nd << endl; node2Files(0);exit(1);}
	d4=LG[(i+1)%3][nd2]; id4=(i+1)%3;
	d5=LG[(i+2)%3][nd2];id5=(i+2)%3;
	if(d4<d5) {echange(&d4,&d5); id5=id4; id4=(i+2)%3;}
	
	if(d1==6 && d3>=1 && d2>=2 && d4>=2 && d5>=1) {supprPath(nd,id); f=true;}// H(1,2,6,2,1)
	else if(d1==5 && d3>=1 && d2>=2 && d4>=3 && d5>=2) {supprPath(nd,id); f=true;}// H(1,2,5,3,2)
	else if(d1==5 && d3>=1 && d2>=4 && d4>=2 && d5>=2) {supprPath(nd,id); f=true;}// H(1,4,5,2,2)
	else if(d1==5 && d3>=2 && d2>=2 && d4>=2 && d5>=2) {supprPath(nd,id); f=true;}// H(2,2,5,2,2)
	else if(d1==4 && d3>=2 && d2>=2 && d4>=3 && d5>=2) {supprPath(nd,id); f=true;}// H(2,2,4,3,2)
	else if(d1==4 && d3>=1 && d2>=2 && d4>=4 && d5>=3) {supprPath(nd,id); f=true;}// H(1,2,4,4,3)
	else if(d1==4 && d3>=1 && d2>=4 && d4>=3 && d5>=3) {supprPath(nd,id); f=true;}// H(1,4,4,3,3)
	else if(d1==3 && d3>=2 && d2>=3 && d4>=4 && d5>=3) {supprPath(nd,id); f=true;}// H(2,3,3,4,3)
	else if(d1==3 && d3>=2 && d2>=4 && d4>=3 && d5>=3) {supprPath(nd,id); f=true;}// H(2,4,3,3,3)
	if(f) { if(verb) cout << "Hhandle (" << d3 << "," << d2 << "," << d1 << "," << d4 << "," << d5 << ") from " << nd << endl; return true;}
	
	if(hcycle && testChandle(nd,nd2,id2,id3,id4,id5,d1,d2,d3,d4,d5)) {supprPath(nd,id); f=true; return true;}
	
	if(hsecond) // test for H''-handle on the side nd => d2,d3
	{
	  int nd3=NG[id2][nd]; 
	  i=0; 
	  while(i<3 && (NG[i][nd3]!=nd || LG[i][nd3]!=d2)) i++;
	  if(i==3) {cerr << "Problem neighbor Hhandle " << nd3 << " not a neighbor of " << nd << endl; node2Files(0);}
	  d6=LG[(i+1)%3][nd3];
	  d7=LG[(i+2)%3][nd3];
	  if(d6<d7) {echange(&d6,&d7);}
	
	  int nd4=NG[id3][nd]; 
	  i=0; 
	  while(i<3 && (NG[i][nd4]!=nd || LG[i][nd4]!=d3)) i++;
	  if(i==3) {cerr << "Pb neighbor Hhandle " << nd4 << " not a neighbor of " << nd << endl; node2Files(0);}
	  d8=LG[(i+1)%3][nd4];
	  d9=LG[(i+2)%3][nd4];
	  if(d8<d9) {echange(&d8,&d9);}
	  
	  if(d1==6 && d3==1 && d2==1 && d4>=2 && d5==1 && ((d6>=4 && d7>=4 && d6+d7>=9) || (d8>=4 && d9>=4 && d8+d9>=9))) {supprPath(nd,id); f=true;}// H''(1,2,6,1,1,1,1,5,4)
	  else if(d1==6 && d3==1 && d2==1 && d4>=2 && d5==1 && ((d6>=4 && d7>=4 && d8>1 && d9>1 && d8+d9>=5) || (d8>=4 && d9>=4 && d6>1 && d7>1 && d6+d7>=5))) {supprPath(nd,id); f=true;}// H''(1,2,6,1,1,2,3,4,4)
	  
	  else if(d1==5 && d3==1 && d2==1 && d4>=2 && d5>=2 && ((d6>=4 && d7>=4 && d8>1 && d9>1 && d8+d9>=5) || (d8>=4 && d9>=4 && d6>1 && d7>1 && d6+d7>=5))) {supprPath(nd,id); f=true;}// H''(2,2,5,1,1,2,3,4,4)
	  else if(d1==5 && d3>=2 && d2>=2 && d4>=2 && d5>=1 && d6+d7+d8+d9>=5) {supprPath(nd,id); f=true;}// H''(1,2,5,2,2,1,1,2,1)
	  else if(d1==5 && d3>=1 && d2>=2 && d4>=2 && d5>=1 && (d6>2 || d7>2) && d8>3 && d9>3 && d6+d7+d8+d9>=12) {supprPath(nd,id); f=true;}// H''(1,2,5,2,1,1,3,4,4)
	  else if(d1==5 && d3>=1 && d2>=3 && d4>=2 && d5>=1 && d6>1 && d7>1 && d8>1 && d9>1 && d6+d7>4 && d8+d9>4) {supprPath(nd,id); f=true;}// H''(1,2,5,3,1,2,3,2,3)
	  else if(d1==5 && d3>=1 && d2>=4 && d4>=2 && d5>=1 && d6>1 && d7>1 && d8>1 && d9>1) {supprPath(nd,id); f=true;}// H''(1,2,5,4,1,2,2,2,2)
	  else if(d1==4 && d3>=2 && d2>=4 && d4>=2 && d5>=1 && d8>1 && d9>1) {supprPath(nd,id); f=true;}// H''(1,2,4,4,2,1,1,2,2)
	  else if(d1==4 && d3>=2 && d2>=4 && d4>=2 && d5>=1 && d6>1 && d7>1 && d8+d9>2) {supprPath(nd,id); f=true;}// H''(1,2,4,4,2,2,2,2,1)
	  else if(d1==4 && d3>=2 && d2>=4 && d4>=2 && d5>=1 && d6>1 && d7>1 && d6+d7>4 && d8>1 && d9>1 && d8+d9>4) {supprPath(nd,id); f=true;}// H''(1,2,4,2,2,2,3,2,3)
	  else if(d1==4 && d3>=1 && d2>=4 && d4>=2 && d5>=2 && d6>1 && d7>1 && d8>1 && d9>1 && d8+d9>4) {supprPath(nd,id); f=true;}// H''(2,2,4,4,1,2,2,3,2)
	  else if(d1==4 && d3>=1 && d2>=3 && d4>=4 && d5>=2 && d6>1 && d7>1 && d8>1 && d9>1 && d8+d9>4) {supprPath(nd,id); f=true;}// H''(2,4,4,3,1,2,2,3,2)
	  else if(d1==4 && d3>=1 && d2>=2 && d4>=4 && d5>=2 && d6>1 && d7>1 && d8>2 && d9>2) {supprPath(nd,id); f=true;}// H''(2,4,4,2,1,2,2,3,3)
	  else if(d1==4 && d3>=2 && d2>=4 && d4>=2 && d5>=2 && d6>1 && d7>1 && d8>1 && d9>1 && d8+d9>4) {supprPath(nd,id); f=true;}// H''(2,2,4,2,2,2,2,3,2)
	  else if(d1==2 && d3>=3 && d2>=3 && d4>=3 && d5>=3 && d6+d7>2 && d8>1 && d9>1) {supprPath(nd,id); f=true;}// H''(3,3,2,3,3,1,2,2,2)
	  
	  if(f) { if(verb) cout << "Hhandle'' (" << d5 << "," << d4 << "," << d1 << "," << d2 << "," << d3 << "," << d6 << "," << d7 << "," << d8 << "," << d9 << ") from " << nd << endl; return true;}
	}
	return false;
}

bool loop(int nd)	// if there is a loop at node nd, remove it
{	
	int nd0=NG[0][nd], nd1=NG[1][nd], nd2=NG[2][nd]; 	
	if(nd0==nd1 && nd1==nd2) {NG[0][nd]=NG[0][nd0]=-1; if(verb) cout << "suppression of Tripod"<< nd << "," << nd0 << endl; return true;}
	if(nd0!=nd && nd1!=nd && nd2!=nd) return false;
	if(verb) cout << "Loop " << nd << ":" << nd0 << "," << nd1 << "," << nd2 << endl;
	if(nd0==nd && nd1==nd) { reconnectVois(nd,2); NG[0][nd2]=-1; } 
	if(nd0==nd && nd2==nd) { reconnectVois(nd,1); NG[0][nd1]=-1; } 
	if(nd1==nd && nd2==nd) { reconnectVois(nd,0); NG[0][nd0]=-1; } 
	NG[0][nd]=-1;
	if(verb) cout << " loop removed around " << nd << endl; 
	return true;
}


int nbNodes(int d)	// return the number of vertices of degree d
{ int cpt=0;
  for(int i=0; i<l ;i++)
     {for(int j=0; j<h ;j++)
	if(S[i][j] && degree(i,j)==d) cpt++;
     }
  return cpt;
}





int main(int argc, char **argv)	// main function
{ int count=0;
  if(argc!=7) exit(2);
  long moy=0;
  h=atoi(argv[1]); 
  l=atoi(argv[2]);
  int iter=atoi(argv[3]); 
  verb=atoi(argv[4]);
  bool trouve=false, stop=false, modif=false, modifb=false, print=false, hex; int u=0, mean=0,cptH=0;
  //srand(time(NULL));
   
  for(int i=0; i<l ;i++)
     for(int j=0;j<h; j++)
	{S[i][j]=false; POS[i][j]=-1;}
  srand(iter);
  n=genereGraph(atoi(argv[5]), atoi(argv[6]));
  cout << "Graph of " << n << " vertices " << endl;
  n-=supprDeg1Nodes();
  nn=nbNodes(3);
  NG[0]=new int[nn]; NG[1]=new int[nn]; NG[2]=new int[nn];
  LG[0]=new int[nn]; LG[1]=new int[nn]; LG[2]=new int[nn];
  if(verb) cout << "==> there are " << nn << " degree-3 nodes among " << n<< endl;
 
  convertToNodegraph();
  
  
  int nbn=nn;

  while(nbn>0 && !stop)
  {
    modif=false;
    for(int i=0;i<nn;i++)
    {
      if(NG[0][i]>-1)
        { 
          if(loop(i)) {modif=true; modifb=true; nbn-=2;}
          else if(Phandle(i)) {modif=true; modifb=true; nbn-=2;}
          else if(Shandle(i,false,false)) {modif=true; modifb=true; nbn-=2; }
    	  else if (LG[0][i]>=2 && Hhandle(i,0,true,false)) {modif=true; modifb=true; nbn-=2; }
    	  else if (LG[1][i]>=2 && Hhandle(i,1,true,false)) {modif=true; modifb=true; nbn-=2; }
    	  else if (LG[2][i]>=2 && Hhandle(i,2,true,false)) {modif=true; modifb=true; nbn-=2; }
	  else if(PPhandle(i)) {modif=true; modifb=true; nbn-=2;}
	  
	  if(!isGood()) {stop=true; printNeighbors(); break;}
	  modifb=false;
    	} 
    }
    if(!modif) 
      for(int i=0;i<nn;i++)
        if(NG[0][i]>-1 && PPhandle(i)) {modif=true; nbn-=2;break;} 
    if(!modif) stop=true;
    
  }
  
  cout << "After reduction: " << nbn << " degree-3 nodes among " << nn<< endl;
  if(nbn>0) {cout << "Graph not completely reduced" << endl; node2Files(iter);} 
 
 return 0;
}

 
