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

#include"cylinder_z.h"
#include"lexer.h"
#include"dive.h"

cylinder_z::cylinder_z(lexer *pp, int rank, int side) : eps(1.0e-5*pp->dx), psi(1.0e-9*pp->dx)
{
    p=pp;
    
    cylinder_z_rank = rank;
    cylinder_z_side = side;
    cylinder_z_type = 33;

    xm=p->O33_xm[rank];
    ym=p->O33_ym[rank];
    r=p->O33_r[rank];

    xs = xm-r;
    xe = xm+r;

    ys = ym-r;
    ye = ym+r;

    zs = -1.0e12;
    ze = 1.0e12;

    if(xe>=xs)
    {
    xsmall=xs;
    xlarge=xe;
    }

    if(xe<xs)
    {
    xsmall=xe;
    xlarge=xs;
    }

    if(ye>=ys)
    {
    ysmall=ys;
    ylarge=ye;
    }

    if(ye<ys)
    {
    ysmall=ye;
    ylarge=ys;
    }

    if(ze>=zs)
    {
    zsmall=zs;
    zlarge=ze;
    }

    if(ze<zs)
    {
    zsmall=ze;
    zlarge=zs;
    }

    cylinder_z_zdir=0;
}

cylinder_z::~cylinder_z()
{
}

void cylinder_z::read(lexer *p, dive *a, int& rankcount)
{

}

double cylinder_z::fx(lexer *p, dive *a, int ii, int jj, int kk)
{

    xn = p->XN[II]-xm;
    yn = p->YN[JJ]-ym;

    if(xn>=0.0)
    signx=1.0;

    if(xn<0.0)
    signx=-1.0;

    root=r*r - yn*yn;
    root = root>=-psi?root:1.0e20;
	
	if(root>=-psi  && root<psi)
		root=0.0;

    val = signx*sqrt(root) + xm;

    return val;
}

double cylinder_z::fy(lexer *p, dive *a, int ii, int jj, int kk)
{
    xn = p->XN[II]-xm;
    yn = p->YN[JJ]-ym;

    if(yn>=0.0)
    signy=1.0;

    if(yn<0.0)
    signy=-1.0;

    root=r*r - xn*xn;
    root = root>=-psi?root:1.0e20;
	
	if(root>=-psi  && root<psi)
		root=0.0;

    val = signy*sqrt(root) + ym;

    return val;
}

double cylinder_z::fz(lexer *p, dive *a, int ii, int jj, int kk)
{
    val=p->ZN[KK];

    return val;
}

double cylinder_z::cfx(lexer *p, dive *a, int ii, int jj, int kk)
{
    xc = p->XP[II]-xm;
    yc = p->YP[JJ]-ym;

    if(xc>=0.0)
    signx=1.0;

    if(xc<0.0)
    signx=-1.0;

    root=r*r - yc*yc;
    root = root>=-psi?root:1.0e20;
	
	if(root>=-psi  && root<psi)
		root=0.0;

    val = signx*sqrt(root) + xm;

    return val;
}

double cylinder_z::cfy(lexer *p, dive *a, int ii, int jj, int kk)
{
    xc = p->XP[II]-xm;
    yc = p->YP[JJ]-ym;

    if(yc>=0.0)
    signy=1.0;

    if(yc<0.0)
    signy=-1.0;

    root=r*r - xc*xc;
	root = root>=-psi?root:1.0e20;
	
	if(root>=-psi  && root<psi)
		root=0.0;

    val = signy*sqrt(root) + ym;

    return val;
}

double cylinder_z::cfz(lexer *p, dive *a, int ii, int jj, int kk)
{
    val=p->ZP[KK];

    return val;
}

double cylinder_z::xstart()
{
    return xs;
}

double cylinder_z::xend()
{
    return xe;
}

double cylinder_z::ystart()
{
    return ys;
}

double cylinder_z::yend()
{
    return ye;
}

double cylinder_z::zstart()
{
    return zs;
}

double cylinder_z::zend()
{
    return ze;
}

int cylinder_z::type()
{
    return cylinder_z_type;
}

int cylinder_z::rank()
{
    return cylinder_z_rank;
}

int cylinder_z::side()
{
    return cylinder_z_side;
}

int cylinder_z::neighbor1()
{
    return nb1;
}

int cylinder_z::neighbor2()
{
    return nb2;
}

int cylinder_z::xdir(int ii, int jj, int kk)
{
    xn = p->XP[II];

    if(xn>=xm)
    cylinder_z_xdir=1;

    if(xn<xm)
    cylinder_z_xdir=4;

    return cylinder_z_xdir;
}

int cylinder_z::ydir(int ii, int jj, int kk)
{
    yn = p->YP[JJ];

    if(yn>=ym)
    cylinder_z_ydir=3;

    if(yn<ym)
    cylinder_z_ydir=2;

    return cylinder_z_ydir;
}

int cylinder_z::zdir(int ii, int jj, int kk)
{
    return cylinder_z_zdir;
}

int cylinder_z::inbound(int ii,int jj, int kk)
{
    //double posx,posy,posz;
    int bound=1;
/*
    posx = p->XP[II];
    posy = p->YP[JJ];
    posz = p->ZP[KK];

    if(posx<xsmall || posx>xlarge || posy<ysmall || posy>ylarge || posz<zsmall || posz>zlarge)
    bound=0;
*/
    return bound;
}

int cylinder_z::inbound_onnode(int ii,int jj, int kk)
{
    double rad[4];
    double x[4];
    double y[4];
    int bound=0;

    x[0] = p->XN[II]-xm;
    y[0] = p->YN[JJ]-ym;

    x[1] = p->XN[IIP1]-xm;
    y[1] = p->YN[JJ]-ym;

    x[2] = p->XN[IIP1]-xm;
    y[2] = p->YN[JJP1]-ym;

    x[3] = p->XN[II]-xm;
    y[3] = p->YN[JJP1]-ym;

    count=0;
    for(n=0;n<4;++n)
    {
    rad[n] = sqrt(x[n]*x[n] + y[n]*y[n]);

    if(rad[n]<=r+eps)
    ++count;
    }
/*
    if(k==3)
    {
    cout<<ii<<" "<<jj<<" "<<kk<<" . "<<i<<" "<<j<<"  "<<k<<"  "<<"RADIUS: "<<count<<"  "<<rad[0]<<"  "<<rad[1]<<"  "<<rad[2]<<"  "<<rad[3]<<endl;
    cout<<x[0]+xm<<" "<<y[0]+ym<<" . "<<x[1]+xm<<" "<<y[1]+ym<<" . "<<x[2]+xm<<" "<<y[2]+ym<<" . "<<x[3]+xm<<" "<<y[3]+ym<<" . "<<endl<<endl;
    }
*/
    if(count==2 || count==3)
    bound=1;

    return bound;
}

int cylinder_z::flag_val(int ii, int jj, int kk)
{
    int ival=1;
    double rad;

    xc = p->XP[II]-xm;
    yc = p->YP[JJ]-ym;

    rad = sqrt(xc*xc + yc*yc);

    if(rad<r)
    ival=-21;

    return ival;
}
