/*--------------------------------------------------------------------
DIVEMesh
Copyright 2008-2019 Hans Bihs

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

#include"data.h"
#include"dive.h"
#include"lexer.h"
#include"kriging.h"

data::data(lexer *p, dive *a) 
{

    for(n=0; n<p->D10; ++n)
    {
    p->D10_x[n] *= p->D12_x;
    p->D10_y[n] *= p->D12_y;
    }

    for(n=0; n<p->D10; ++n)
    {
    p->D10_x[n] += p->D11_x;
    p->D10_y[n] += p->D11_y;
    }
	
	if(p->D23==1)
	for(n=0; n<p->D10; ++n)
	p->D10_data[n] *= -1.0;
}

data::~data()
{
}

void data::start(lexer* p, dive* a)
{
    cout<<"data"<<endl;
	
	if(p->D14==1)
	{
	cout<<"inverse distance"<<endl;	
		
    XYLOOP
    a->data(i,j) = inverse_dist_2D(p,a);
	}
	
	if(p->D14==2)
	{
	kriging krig(p,a);
	
	krig.start(p,a);
	}
	
	k=z1;
	XYLOOP
	if(a->flag(i,j,k)>0)
	{
	
		if(a->flag(i-1,j,k)<0)
		a->data(i-1,j) = a->data(i,j);
		
		if(a->flag(i+1,j,k)<0)
		a->data(i+1,j) = a->data(i,j);
		
		if(a->flag(i,j-1,k)<0)
		a->data(i,j-1) = a->data(i,j);
		
		if(a->flag(i,j+1,k)<0)
		a->data(i,j+1) = a->data(i,j);
	}
	
	
	k=z1;
	for(n=0;n<p->D15;++n)
	XYLOOP
	if(a->flag(i,j,k)>0)
    a->data(i,j) = p->D16*a->data(i,j) + 0.25*(1.0-p->D16)*(a->data(i-1,j) + a->data(i+1,j) + a->data(i,j-1) + a->data(i,j+1)); 
	
}







