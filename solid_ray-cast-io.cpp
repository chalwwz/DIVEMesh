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

#include"solid.h"
#include"dive.h"
#include"lexer.h"

void solid::ray_cast_io(lexer* p, dive* a, int ts, int te)
{
	double ys,ye,zs,ze;
	double Px,Py,Pz;
	double Qx,Qy,Qz;
	double Rx,Ry,Rz;
	double Ax,Ay,Az;
	double Bx,By,Bz;
	double Cx,Cy,Cz;
	double PQx,PQy,PQz;
	double PAx,PAy,PAz;
	double PBx,PBy,PBz;
	double PCx,PCy,PCz;
	double Mx,My,Mz;
	int js,je,ks,ke;
	double u,v,w;
	double denom;
    double psi = 1.0e-6*p->DXM;

	
    MALOOP
	{
	cutl(i,j,k)=0;
	cutr(i,j,k)=0;
	}

	for(n=ts; n<te; ++n)
	{
	Ax = p->tri_x[n][0];
	Ay = p->tri_y[n][0];
	Az = p->tri_z[n][0];
		
	Bx = p->tri_x[n][1];
	By = p->tri_y[n][1];
	Bz = p->tri_z[n][1];
		
	Cx = p->tri_x[n][2];
	Cy = p->tri_y[n][2];
	Cz = p->tri_z[n][2];	
	
	ys = MIN3(Ay,By,Cy);
	ye = MAX3(Ay,By,Cy);
	
	zs = MIN3(Az,Bz,Cz);
	ze = MAX3(Az,Bz,Cz);
    
    
    js = p->posf_j(ys);
    je = p->posf_j(ye);
    
    ks = p->posf_k(zs);
    ke = p->posf_k(ze);
    
    ys = MIN3(Ay,By,Cy) - epsi*p->YP[js + marge-1];
	ye = MAX3(Ay,By,Cy) + epsi*p->YP[je + marge+1];
	
	zs = MIN3(Az,Bz,Cz) - epsi*p->ZP[ks + marge-1];
	ze = MAX3(Az,Bz,Cz) + epsi*p->ZP[ke + marge+1];
    
    
    
    js = p->posf_j(ys);
    je = p->posf_j(ye);
    
    ks = p->posf_k(zs);
    ke = p->posf_k(ze);
    
	js = MAX(js,0);
	je = MIN(je,p->knoy);
	
	ks = MAX(ks,0);
	ke = MIN(ke,p->knoz);
    
    
		for(j=js;j<je;j++)
		for(k=ks;k<ke;k++)
		{
		Px = p->xmin-10.0*p->DXM;
		Py = p->YP[JP]+psi + p->ymin;
		Pz = p->ZP[KP]+psi + p->zmin;
		
		Qx = p->xmax+10.0*p->DXM;
		Qy = p->YP[JP]+psi + p->ymin;
		Qz = p->ZP[KP]+psi + p->zmin;
		
		
		PQx = Qx-Px;
		PQy = Qy-Py;
		PQz = Qz-Pz;
		
		PAx = Ax-Px;
		PAy = Ay-Py;
		PAz = Az-Pz;
		
		PBx = Bx-Px;
		PBy = By-Py;
		PBz = Bz-Pz;
		
		PCx = Cx-Px;
		PCy = Cy-Py;
		PCz = Cz-Pz;
		
		// uvw
		Mx = PQy*Pz - PQz*Py;
		My = PQz*Px - PQx*Pz;
		Mz = PQx*Py - PQy*Px;

		
		u = PQx*(Cy*Bz - Cz*By) + PQy*(Cz*Bx - Cx*Bz) + PQz*(Cx*By - Cy*Bx)
		  + Mx*(Cx-Bx) + My*(Cy-By) + Mz*(Cz-Bz);
		  
		v = PQx*(Ay*Cz - Az*Cy) + PQy*(Az*Cx - Ax*Cz) + PQz*(Ax*Cy - Ay*Cx)
		  + Mx*(Ax-Cx) + My*(Ay-Cy) + Mz*(Az-Cz);
		  
		w = PQx*(By*Az - Bz*Ay) + PQy*(Bz*Ax - Bx*Az) + PQz*(Bx*Ay - By*Ax)
		  + Mx*(Bx-Ax) + My*(By-Ay) + Mz*(Bz-Az);
		
		
		int check=1;
		if(u==0.0 && v==0.0 && w==0.0)
		check = 0;
		
			if(((u>=0.0 && v>=0.0 && w>=0.0) || (u<0.0 && v<0.0 && w<0.0)) && check==1)
			{
			denom = 1.0/(u+v+w);
			u *= denom;
			v *= denom;
			w *= denom;
			
			Rx = u*Ax + v*Bx + w*Cx;
			Ry = u*Ay + v*By + w*Cy;
			Rz = u*Az + v*Bz + w*Cz;


				for(i=0;i<=a->knox;++i)
				{
				if(p->XP[IP]<=Rx-p->xmin)
				cutr(i,j,k) += 1;
				
				if(p->XP[IP]>Rx-p->xmin)
				cutl(i,j,k) += 1;
				}
			}
		}
	}
	
	LOOP
	if((cutl(i,j,k)+1)%2==0  && (cutr(i,j,k)+1)%2==0)
    {
	a->solid(i,j,k)=-1;
    a->solid_dist(i,j,k)=-fabs(a->solid_dist(i,j,k));
    }

	count=0;
	LOOP
	if(a->solid(i,j,k)>0)
	++count;

	
	cout<<"Number of active cells after solid: "<<count<<endl;
	
}