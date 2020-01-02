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

#include"solid.h"
#include"dive.h"
#include"lexer.h"


void solid::ray_cast_correct(lexer* p, dive* a, int ts, int te)
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
	int ir;
	double u,v,w;
	double denom;	
	int insidecheck;

    
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
	
	ys = MIN3(Ay,By,Cy) - epsi*p->dx;
	ye = MAX3(Ay,By,Cy) + epsi*p->dx;
	
	zs = MIN3(Az,Bz,Cz) - epsi*p->dx;
	ze = MAX3(Az,Bz,Cz) + epsi*p->dx;
	
	js = int((ys-p->ymin)/p->dx);
	je = int((ye-p->ymin)/p->dx);
	
	ks = int((zs-p->zmin)/p->dx);
	ke = int((ze-p->zmin)/p->dx);
	
	js = MAX(js,0);
	je = MIN(je,p->knoy);
	
	ks = MAX(ks,0);
	ke = MIN(ke,p->knoz);
	
	
		for(j=js;j<je;j++)
		for(k=ks;k<ke;k++)
		{
		Px = p->xmin-10.0*p->dx ;
		Py = (double(j)+0.500001)*p->dx + p->ymin;
		Pz = (double(k)+0.500001)*p->dx + p->zmin;
		
		Qx = p->xmax+10.0*p->dx ;
		Qy = (double(j)+0.500001)*p->dx + p->ymin;
		Qz = (double(k)+0.500001)*p->dx + p->zmin;
		
		
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
 
			for(i=0;i<=p->knox;++i)
            if(fabs(a->solid_dist(i,j,k)<1.6*p->dx))
            {
            if(fabs(a->solid_dist(i,j,k))>fabs(Rx-p->posc_x()));
            cout<<"DIST: "<<fabs(a->solid_dist(i,j,k))-fabs(Rx-p->posc_x())<<endl;
			
            
            
            //a->solid_dist(i,j,k)=MIN(fabs(Rx-p->posc_x()),a->solid_dist(i,j,k));
            }
			
			}
		}
	}
	
}