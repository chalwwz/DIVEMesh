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

#include"inverse_dist_local.h"
#include"dive.h"
#include"lexer.h"

void inverse_dist_local::setup(lexer *p, dive *a, int Np, double *Fx, double *Fy, double *Fz)
{
     
    xmin=+1.0e19;
    ymin=+1.0e19;
    zmin=+1.0e19;

    xmax=-1.0e19;
    ymax=-1.0e19;
    zmax=-1.0e19;

    for(n=0;n<Np;++n)
    {
    xmax=MAX(xmax,Fx[n]);
    xmin=MIN(xmin,Fx[n]);
    ymax=MAX(ymax,Fy[n]);
    ymin=MIN(ymin,Fy[n]);
    zmax=MAX(zmax,Fz[n]);
    zmin=MIN(zmin,Fz[n]);
    }
            
    // Grid
    dd = 3;
    
    Nx = MAX(p->knox+2*dd+1,int((xmax-xmin)/p->DXM)+2*dd+1);
    Ny = MAX(p->knoy+2*dd+1,int((ymax-ymin)/p->DXM)+2*dd+1);

    p->Iarray(ptnum,Nx,Ny);
    
    for(r=0;r<Nx;++r)
    for(s=0;s<Ny;++s)
    ptnum[r][s]=0;

    for(n=0;n<Np;++n)
    {
    ic = p->posc_i(Fx[n]);
    jc = p->posc_j(Fy[n]);
    
    //ic = int((Fx[n]-p->xmin)/p->DXM);
    //jc = int((Fy[n]-p->ymin)/p->DXM);
    
    ICFLAG
    ++ptnum[ic+dd][jc+dd];
    }
    
    
    p->Iarray(ptid,Nx,Ny, ptnum);
    
    for(r=0;r<Nx;++r)
    for(s=0;s<Ny;++s)
    ptnum[r][s]=0;
    
    for(r=0;r<Nx;++r)
    for(s=0;s<Ny;++s)
    for(t=0;t<ptnum[r][s];++t)
    ptid[r][s][t]=-1;

    
    for(n=0;n<Np;++n)
    {
    ic = p->posc_i(Fx[n]);
    jc = p->posc_j(Fy[n]);
    
    //ic = int((Fx[n]-p->xmin)/p->DXM);
    //jc = int((Fy[n]-p->ymin)/p->DXM);
    
    ICFLAG
    {
    ptid[ic+dd][jc+dd][ptnum[ic+dd][jc+dd]]=n;
    ++ptnum[ic+dd][jc+dd];
    }
    }
    
    
    // Radius
    Dmax=sqrt(pow(p->xmax-p->xmin,2.0)+pow(p->ymax-p->ymin,2.0));
    R = 0.25*Dmax*sqrt(19.0/Np);
    
    dij = MAX(int(R/(p->DXM)),p->G17);
    
    cout<<"IDW local "<<" Nx: "<<Nx<<" Ny: "<<Ny<<" R: "<<R<<" dij: "<<dij<<endl;
}






