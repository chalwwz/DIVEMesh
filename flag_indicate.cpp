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

#include"flag_indicate.h"
#include"dive.h"
#include"lexer.h"
#include"entity.h"

flag_indicate::flag_indicate() : eps(1.0e-12)
{
}

flag_indicate::~flag_indicate()
{
}

void flag_indicate::indicate(lexer* p, dive* a, entity *pent)
{
	
     xstart=pent->xstart();
     xend=pent->xend();

     ystart=pent->ystart();
     yend=pent->yend();

     zstart=pent->zstart();
     zend=pent->zend();

     // cout<<pent->side()<<" xs: "<<xstart<<" xe: "<<xend<<" ys: "<<ystart<<" ye: "<<yend<<" zs: "<<zstart<<" ze: "<<zend<<endl;

    if(xend>=xstart)
    {
    xdir=1;
    xsmall=xstart;
    xlarge=xend;
    }

    if(xend<xstart)
    {
    xdir=-1;
    xsmall=xend;
    xlarge=xstart;
    }

    if(yend>=ystart)
    {
    ydir=1;
    ysmall=ystart;
    ylarge=yend;
    }

    if(yend<ystart)
    {
    ydir=-1;
    ysmall=yend;
    ylarge=ystart;
    }

    side=pent->side();

    LOOP
     {
     xc0 = (double(i)+0.5)*p->dx;
     yc0 = (double(j)+0.5)*p->dx;
     zc0 = p->ZP[KP];

     xc1 = (double(i)+1.5)*p->dx;
     yc1 = (double(j)+1.5)*p->dx;
     zc1 = (double(k)+1.5)*p->dx;

     xx = pent->cfx(p,a,i,j,k);
     yy = pent->cfy(p,a,i,j,k);
/*
           if(k==3 && xc0>=2.99 && xc0<3.1 && yc0>=0.95 && yc0<1.1)
                    {
					cout<<"side: "<<side<<" xdir: "<<xdir<<" ydir: "<<ydir<<endl;
                     cout<<q<<" i: "<<i<<" j: "<<j<<" k: "<<k;
                     cout<<" xsmall: "<<xsmall<<" xlarge: "<<xlarge<<endl;
                     cout<<" ysmall: "<<ysmall<<" ylarge: "<<ylarge<<endl;
                     cout<<" xc0: "<<xc0<<" yc0: "<<yc0<<" zc0: "<<zc0;
                     cout<<" xc1: "<<xc1<<" yc1: "<<yc1<<" zc1: "<<zc0;
                     cout<<" xx: "<<xx<<" yy: "<<yy<<endl<<endl;
                     }*/

         if(xc0>=xsmall-p->dx-eps && xc0<=xlarge+eps)
         if(yc0>=ysmall-p->dx-eps && yc0<=ylarge+eps)
         if(zc0>=zstart-p->dx-eps && zc0<=zend+eps)
         {
             // x-dir
             if(xx>=xc0-eps && xx<=xc1+eps)
             {
                if((ydir==1 && side==1) || (ydir==-1 && side==2))
                {
                if(a->flag(i,j,k)==0)
                a->flag(i,j,k)     = -21;
                //if(xc1>=xsmall-p->dx-eps && xc1<=xlarge+p->dx+eps)
                a->flag(i+1,j,k) =   1;
                }

                if((ydir==-1 && side==1) || (ydir==1 && side==2))
                {
                a->flag(i,j,k)     =      1;
                //if(xc1>=xsmall-p->dx-eps && xc1<=xlarge+p->dx+eps)

                if(a->flag(i+1,j,k)==0)
                a->flag(i+1,j,k) =  -21;
                }
				
				//if(a->flag(121,39,3)>0)
				//cout<<"FLAG XDIR  &&&&&&&&"<<endl;
             }

             // y-dir
             if(yy>=yc0-eps && yy<=yc1+eps)
             {


                if((xdir==1 && side==1) || (xdir==-1 && side==2))
                {
                a->flag(i,j,k)     =     1;
                //if(yc1>=ysmall-p->dx-eps && yc1<=ylarge+p->dx+eps)

                if(a->flag(i,j+1,k)==0)
                a->flag(i,j+1,k) = -21;
                }

                if((xdir==-1 && side==1) || (xdir==1 && side==2))
                {
                if(a->flag(i,j,k)==0)
                a->flag(i,j,k)     =  -21;
                //if(yc1>=ysmall-p->dx-eps && yc1<=ylarge+p->dx+eps)
                a->flag(i,j+1,k) =     1;
                }
				//if(a->flag(121,39,3)>0)
				//cout<<"FLAG YDIR  &&&&&&&&   "<<" yy: "<<yy<<" yc0: "<<yc0<<" yc1: "<<yc1<<endl;
             }
        }
     }
}

void flag_indicate::finalize(lexer* p, dive* a)
{
    MALOOP
    OUTCHECK
    a->flag(i,j,k)=0;

    for(n=0;n<100;++n)
    LOOP
    if(a->flag(i,j,k)==0)
    if(a->flag(i-1,j,k)>=0 && a->flag(i+1,j,k)>=0 && a->flag(i,j-1,k)>=0 && a->flag(i,j+1,k)>=0)
    if(a->flag(i-1,j,k)>0 || a->flag(i+1,j,k)>0 || a->flag(i,j-1,k)>0 || a->flag(i,j+1,k)>0)
    a->flag(i,j,k)=1;

    MALOOP
    OUTCHECK
    a->flag(i,j,k)=-21;

    MALOOP
    if(a->flag(i,j,k)==0)
    a->flag(i,j,k)=-21;

    ofstream result;
	result.open("flag_indicate.txt");

	k=0;
	ILOOP
	{
	    JLOOP
	    {
        if(a->flag(i,j,k)<0)
        result<<0<<" ";

        if(a->flag(i,j,k)>0)
        result<<1<<" ";

        if(a->flag(i,j,k)==0)
        result<<0<<" ";

	    }
	    result<<" $ "<<i<<endl;

	}
	
	i=121;
	j=39;
	k=3;
}





