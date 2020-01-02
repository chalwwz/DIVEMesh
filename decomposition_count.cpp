/*--------------------------------------------------------------------
DIVEMesh
Copyright 2008-2020 Hans Bihs

This file is part of DIVEMesh.

DIVEMesh is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------
--------------------------------------------------------------------*/

#include"decomposition.h"

void decomp::surfcount(lexer* p,dive* a)
{	
    for(i=0;i<a->surfcount;i++)
    {
    n=a->subgrid(a->surf[i][0],a->surf[i][1],a->surf[i][2]);
    a->wall[n]++;
    }
	

    for(i=0;i<a->para1count;i++)
    {
    n=a->subgrid(a->para1sf[i][0],a->para1sf[i][1],a->para1sf[i][2]);
    a->para1[n]++;
    }

    for(i=0;i<a->para2count;i++)
    {
    n=a->subgrid(a->para2sf[i][0],a->para2sf[i][1],a->para2sf[i][2]);
    a->para2[n]++;
    }

    for(i=0;i<a->para3count;i++)
    {
    n=a->subgrid(a->para3sf[i][0],a->para3sf[i][1],a->para3sf[i][2]);
    a->para3[n]++;
    }

    for(i=0;i<a->para4count;i++)
    {
    n=a->subgrid(a->para4sf[i][0],a->para4sf[i][1],a->para4sf[i][2]);
    a->para4[n]++;
    }

    for(i=0;i<a->para5count;i++)
    {
    n=a->subgrid(a->para5sf[i][0],a->para5sf[i][1],a->para5sf[i][2]);
    a->para5[n]++;
    }

    for(i=0;i<a->para6count;i++)
    {
    n=a->subgrid(a->para6sf[i][0],a->para6sf[i][1],a->para6sf[i][2]);
    a->para6[n]++;
    }
}

void decomp::voidsurfcount(lexer* p,dive* a)
{
    for(i=0;i<a->paravoid1count;i++)
    {
    n=a->subgrid(a->para1void[i][0],a->para1void[i][1],a->para1void[i][2]);
    a->paravoid1[n]++;
    }

    for(i=0;i<a->paravoid2count;i++)
    {
    n=a->subgrid(a->para2void[i][0],a->para2void[i][1],a->para2void[i][2]);
    a->paravoid2[n]++;
    }

    for(i=0;i<a->paravoid3count;i++)
    {
    n=a->subgrid(a->para3void[i][0],a->para3void[i][1],a->para3void[i][2]);
    a->paravoid3[n]++;
    }

    for(i=0;i<a->paravoid4count;i++)
    {
    n=a->subgrid(a->para4void[i][0],a->para4void[i][1],a->para4void[i][2]);
    a->paravoid4[n]++;
    }

    for(i=0;i<a->paravoid5count;i++)
    {
    n=a->subgrid(a->para5void[i][0],a->para5void[i][1],a->para5void[i][2]);
    a->paravoid5[n]++;
    }

    for(i=0;i<a->paravoid6count;i++)
    {
    n=a->subgrid(a->para6void[i][0],a->para6void[i][1],a->para6void[i][2]);
    a->paravoid6[n]++;
    }
}


void decomp::slicesurfcount(lexer* p,dive* a)
{	

    for(i=0;i<a->paraslice1count;i++)
    {
    n=a->subslice(a->paraslice1sf[i][0],a->paraslice1sf[i][1]);
    a->paraslice1[n]++;
    }

    for(i=0;i<a->paraslice2count;i++)
    {
    n=a->subslice(a->paraslice2sf[i][0],a->paraslice2sf[i][1]);
    a->paraslice2[n]++;
    }

    for(i=0;i<a->paraslice3count;i++)
    {
    n=a->subslice(a->paraslice3sf[i][0],a->paraslice3sf[i][1]);
    a->paraslice3[n]++;
    }

    for(i=0;i<a->paraslice4count;i++)
    {
    n=a->subslice(a->paraslice4sf[i][0],a->paraslice4sf[i][1]);
    a->paraslice4[n]++;
    }
}

void decomp::slicecornercount(lexer* p,dive* a)
{	
    for(i=0;i<a->paracoslice1count;i++)
    {
    n=a->paracoslice1sf[i][2];
    a->paracoslice1[n]++;
    }

    for(i=0;i<a->paracoslice2count;i++)
    {
    n=a->paracoslice2sf[i][2];
    a->paracoslice2[n]++;
    }

    for(i=0;i<a->paracoslice3count;i++)
    {
    n=a->paracoslice3sf[i][2];
    a->paracoslice3[n]++;
    }

    for(i=0;i<a->paracoslice4count;i++)
    {
    n=a->paracoslice4sf[i][2];
    a->paracoslice4[n]++;
    }
}

void decomp::ccsurf(lexer* p,dive* a)
{
    //initialize
    for(n=0;n<p->M10+1;n++)
    {
    a->facetnum[n]=0;
    a->ccptnum[n]=0;
    a->ccstatenum[n]=0;
    }
	
	// count and allocate
	pcount=0;
	int maxcount=-1.e9;
    NLOOP
    {
    count=0;
    ++pcount;
    for(n=0;n<a->maxsurf;n++)
    a->ccmem[n]=0;

        SUBLOOP
        if(a->confac(i,j,k)>0)
        {
        q=a->confac(i,j,k);

            for(n=0;n<a->numfac(i,j,k);++n)
            {
                id=a->facet[q][n];

				if(a->ccmem[id]==0)
				{
				 //a->facet[q][n]=count;
                count++;
				}
                

                //if(a->ccmem[id]>0)
                //a->facet[q][n]=a->ccmem[id];

            }
		++a->facetnum[pcount];
        }
	maxcount=MAX(count, maxcount);
    }
	
	ddout<<"maxcout_ccloc: "<<maxcount<<endl;
	
	p->Darray(a->ccloc,p->M10+1,maxcount,3);
	
	//initialize 2
    for(n=0;n<p->M10+1;n++)
    {
    a->facetnum[n]=0;
    a->ccptnum[n]=0;
    a->ccstatenum[n]=0;
    }
	
	// ccloc
    pcount=0;
    NLOOP
    {
    count=0;
    pcount++;
    for(n=0;n<a->maxsurf;n++)
    a->ccmem[n]=0;

        SUBLOOP
        if(a->confac(i,j,k)>0)
        {
        q=a->confac(i,j,k);

            for(n=0;n<a->numfac(i,j,k);++n)
            {
                id=a->facet[q][n];

                if(a->ccmem[id]==0)
                {
                a->ccloc[pcount][count][0]=a->ccpoint[id][0];
                a->ccloc[pcount][count][1]=a->ccpoint[id][1];
                a->ccloc[pcount][count][2]=a->ccpoint[id][2];
                a->ccptnum[pcount]++;
                a->ccmem[id]=count;
                a->facet[q][n]=count;
                count++;
                }

                if(a->ccmem[id]>0)
                a->facet[q][n]=a->ccmem[id];

            }
        a->facetnum[pcount]++;
        }
    }


    LOOP
    {
    n=a->confac(i,j,k);
    q=a->subgrid(i,j,k);

    if(a->ccstate[n]==1)
    a->ccstatenum[q]++;
    }
}

void decomp::cornercount(lexer* p,dive* a)
{

    for(i=0;i<a->paraco1count;i++)
    {
    n=a->para1co[i][3];
    a->paraco1[n]++;
    }

    for(i=0;i<a->paraco2count;i++)
    {
    n=a->para2co[i][3];
    a->paraco2[n]++;
    }

    for(i=0;i<a->paraco3count;i++)
    {
    n=a->para3co[i][3];
    a->paraco3[n]++;
    }

    for(i=0;i<a->paraco4count;i++)
    {
    n=a->para4co[i][3];
    a->paraco4[n]++;
    }

    for(i=0;i<a->paraco5count;i++)
    {
    n=a->para5co[i][3];
    a->paraco5[n]++;
    }

    for(i=0;i<a->paraco6count;i++)
    {
    n=a->para6co[i][3];
    a->paraco6[n]++;
    }
}
