
//  userGraphics.h
//  July 7, 2012
//  YL

#include "globalVariables.h"
#include "globalFunctions.h"
#include "BasicGLPane.h"
#include <stdio.h>
#include "dmRigidBody.hpp"
#include "dmDynamicContactModel.hpp"
#include "dmContactModel.hpp"

void drawArrow(Vector3F & location, Vector3F & direction,double lineWidth, double headWidth, double headLength);
void showActivatedTerrainPatches();
const float RADTODEG = (float)(180.0/M_PI);    // M_PI is defined in math.h
const float DEGTORAD = (float)(M_PI/180.0);



void BasicGLPane::userGraphics2DPrimary()
{
	// for customized text rendering on primary viewport

	// vector<Float> c1u(G_contact->getDynamicContacts()[0]->u[0]);
	//cout<<"c1u - "<<c1u[0]<<endl;

	SpatialVector f;
	dynamic_cast<dmRigidBody *>(G_robot->getLink(0))->getExternalForce(f);

	if (f[3] != 0)
	{
		RotationMatrix  R;
		CartesianVector p;	
		G_robot->getLink(0)->getPose(R,p);
		GLdouble fvp[3];
		gluProject(p[0]+f[3]/100+0.05, p[1], p[2], modelview, projection, vp, &fvp[0], &fvp[1], &fvp[2]);

	
		glColor3f (1,1,1);
		glRasterPos2f(fvp[0], vp[3]-fvp[1]);
		//glRasterPos2f(10, 60);//
		char disp_text[100]= "";
		sprintf(disp_text, "%2.2f", f[3]);
		int len = (int) strlen(disp_text);
		for (int i = 0; i<len; ++i)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, disp_text[i]);
	}
}



void BasicGLPane::userGraphics()
{
	// Plot User Stuff
	{
		// draw ground contact force
		Vector3F box_pos;
		Vector3F fe;
		RotationMatrix  R;
		CartesianVector p;	
		G_robot->getLink(0)->getPose(R,p);
		box_pos= Map<Vector3F>(p);
		SpatialVector f;

		dmRigidBody * rb = dynamic_cast<dmRigidBody *>(G_robot->getLink(0));
		#ifdef USE_DYNAMIC_CONTACT_MODEL
		dmDynamicContactModel * cm = dynamic_cast<dmDynamicContactModel *>(rb->getForce(0));
		#else
		dmContactModel * cm = dynamic_cast<dmContactModel *>(rb->getForce(0));
		#endif

/*		// only for single contact point
		for (int j=0;j<3;j++)
			fe(j) = cm->m_fe_ICS[0][j];

		
		fe = fe/100;
		glColor3f(1.0, 0.0, 0.0);
		drawArrow(box_pos, fe, .005, .01, .03);		
		glColor3f(1.0, 1.0, 1.0);*/

		rb->getExternalForce(f);
		Vector3F ext_f;
		ext_f[0]=f[3]; ext_f[1]=f[4]; ext_f[2]=f[5]; ext_f=ext_f/100;
		glColor3f(0.0, 0.0, 1.0);
		drawArrow(box_pos, ext_f, .005, .01, .03);		
		glColor3f(1.0, 1.0, 1.0);

		
		// show which terrain patch is activated
		/*showActivatedTerrainPatches();
		*/

/*		if (frame->showCoM->IsChecked()) 
		{

		}
		
		
		if (frame->showGRF->IsChecked()) 
		{

		}
		
		if (frame->showNetForceAtGround->IsChecked()) 
		{

		}*/
		
		if (simThread->sim_time>500) {
			glPushMatrix();
			glColor4f(1.0, 0.0, 0.0,0.75);
			glTranslatef(2.18, 2, .15);
			glRotated(-90, 0, 1, 0);
			gluDisk(quadratic,0.0,.05f,32,32);
			glRotated(180, 0, 1, 0);
			gluDisk(quadratic,0.0,.05f,32,32);
			
			glPopMatrix();
		}
		frame->realTimeRatioDisplay->SetLabel(wxString::Format(wxT("RT Ratio: %.2lf"), real_time_ratio));
		
	}
	
}



void drawArrow(Vector3F & location, Vector3F & direction,double lineWidth, double headWidth, double headLength) {
	Vector3F zup; zup << 0,0,1;
	
	Vector3F normedDirection = direction.normalized();
	
	// Note: We need to create a rotation such that the z-axis points in the direction of the 'direction' argument
	//       Thus, we can realize the rotation with a pure x-y axial rotation
	//       The rotation matrix of the rotated frame 'r' to the current frame 'c' (c_R_r) has special form.
	//       It's 3-rd column in particular has form: [ wy*sin(theta) -wx*sin(theta) (1- cos(theta))]'
	//       where theta , [wx wy 0] is the angle, axis of rotation
	
	// Find the rotation angle by comparing the z-axis of the current and rotated frame
	const double cosTheta = zup.dot(normedDirection);
	const double theta = acos(cosTheta);
	const double sinTheta = sin(theta);
	
	
	// Exploit the special form of the rotation matrix (explained above) for find the axis of rotation
	double rX, rY, rZ;
	if(theta > 0) {	
		rX = - normedDirection(1)/sinTheta;
		rY =   normedDirection(0)/sinTheta;
		rZ = 0;
	}
	else {
		rX = 0;
		rY = 0;
		rZ = 1;
	}
	
	// before calling this function, be sure glMatrixMode is GL_MODELVIEW and camera->applyView is called.
	glPushMatrix();
	glTranslatef(location(0), location(1), location(2));
	glRotated(RADTODEG * theta, rX, rY, rZ);
	
	
	double cylinderLength = direction.norm();
	if (cylinderLength > headLength) {
		cylinderLength -= headLength;
	}
	else {
		headLength = cylinderLength ;
		cylinderLength = 0;
	}

	const int detail = 16;
	// Draw Cylinder [along the z axis]
    // gluCylinder(GLUquadricObj *qobj, GLdouble baseRadius, GLdouble topRadius, GLdouble height, GLint slices, GLint stacks)
	//gluCylinder(frame->glPane->quadratic,lineWidth,lineWidth,cylinderLength,detail,detail);
		glBegin(GL_LINES);
		glVertex3f(0.0, 0.0,cylinderLength );
		glVertex3f(0.0, 0.0, 0.0);
		glEnd();	

	//Draw Cylinder Base
	//gluDisk(frame->glPane->quadratic,0,lineWidth,detail,detail);

	gluSphere(frame->glPane->quadratic,.03f,32,32);
	
	glTranslatef(0, 0, cylinderLength);
	//Draw Arrowhead
	gluCylinder(frame->glPane->quadratic,headWidth,0.0f,headLength,detail,detail);
	
	//Draw Arrowhead Base
	gluDisk(frame->glPane->quadratic,lineWidth,headWidth,detail,detail);
	
	glEnd();
	glPopMatrix();
	
}


void showActivatedTerrainPatches()
{
	dmRigidBody * rb;
	rb = dynamic_cast<dmRigidBody *>(G_robot->getLink(0));

	dmContactModel * cm;
	cm = dynamic_cast<dmContactModel *>(rb->getForce(0));
		
	int xdim, ydim;
	Float res;
	dmEnvironment::getEnvironment ()->getTerrainData(xdim, ydim, res);	  	
	int i =0;
	if ( cm->getContactState(i) )
	{


		CartesianVector cpos, cpos_ICS; 
		cm->getContactPoint(i,cpos); //in body CS
		RotationMatrix  R_ICS;
		CartesianVector p_ICS;	
		rb->getPose(R_ICS,p_ICS);
	
		for (int j = 0; j < 3; j++)  // compute the cpos wrt ICS.
		{                        
		 	cpos_ICS[j] = 	 p_ICS[j] +
			              	 R_ICS[j][0]*cpos[0] +
			              	 R_ICS[j][1]*cpos[1] +
			              	 R_ICS[j][2]*cpos[2]; //current contact position in ICS
	  	}
		int xindex = (int) (cpos_ICS[0]/res);
		int yindex = (int) (cpos_ICS[1]/res);

		if (cpos_ICS[0] < 0.0) { xindex--;}
		if (cpos_ICS[1] < 0.0) { yindex--;}

		Float t, u;
		t = (cpos_ICS[0] -
			((float) xindex*res))/res;
		u = (cpos_ICS[1] -
			((float) yindex*res))/res;

		glColor3f(0.1, 0.3, 0.7);
		GLfloat vertex[3][3];
		Float ** depths; 
		dmEnvironment::getEnvironment()->getTerrainDepthsArray(depths);
		// printf("-> %p\n", depths);
		// cout<<"-----^o^-------"<<endl;
		// cout<<xindex<<" "<<yindex<<endl;

		if ( (xindex>=0) && (yindex>=0) && (xindex<xdim-1) && yindex<ydim-1)
		{
			if (t>u)
			{
				vertex[0][0] = ((GLfloat) xindex)*res;
				vertex[0][1] = ((GLfloat) yindex)*res;
				vertex[0][2] = depths[xindex][yindex]+0.0001;		

				vertex[1][0] = ((GLfloat) xindex+1)*res;
				vertex[1][1] = ((GLfloat) yindex)*res;
				vertex[1][2] = depths[xindex+1][yindex]+0.0001;		

				vertex[2][0] = ((GLfloat) xindex+1)*res;
				vertex[2][1] = ((GLfloat) yindex+1)*res;
				vertex[2][2] = depths[xindex+1][yindex+1]+0.0001;		

				glBegin(GL_TRIANGLES);
				glVertex3fv(vertex[0]);
				glVertex3fv(vertex[1]);
				glVertex3fv(vertex[2]);
				glEnd();

			}
			else
			{
				vertex[0][0] = ((GLfloat) xindex)*res;
				vertex[0][1] = ((GLfloat) yindex)*res;
				vertex[0][2] = depths[xindex][yindex]+0.0001;		

				vertex[1][0] = ((GLfloat) xindex+1)*res;
				vertex[1][1] = ((GLfloat) yindex+1)*res;
				vertex[1][2] = depths[xindex+1][yindex+1]+0.0001;	

				vertex[2][0] = ((GLfloat) xindex)*res;
				vertex[2][1] = ((GLfloat) yindex+1)*res;
				vertex[2][2] = depths[xindex][yindex+1]+0.0001;		

				glBegin(GL_TRIANGLES);
				glVertex3fv(vertex[0]);
				glVertex3fv(vertex[1]);
				glVertex3fv(vertex[2]);
				glEnd();
			}
		}
		glColor3f(1.0, 1.0, 1.0);
	}


}

