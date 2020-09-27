#define _USE_MATH_DEFINES
#include<iostream>
#include<fstream>
#include<iomanip>
#include<math.h>

using namespace std;


int main()
{

	ifstream archivo("cerout.dat", ios::in);
	

	float a, px, py, pz, x, y, t;
	float energia;
	float norma;
	float theta_rad;
	float theta_deg;
	double masa;
	float digit1, digit2, digit3, digit4;
	int flag1, flag2;
	
	flag1=0;
	flag2=0;
	
	ofstream salida;
	salida.open("DATOS_PARTICULAS.dat");
	
	while(archivo>>a>>px>>py>>pz>>x>>y>>t)
	{
		if((a>1000)&&(a<9999))
		{
			digit1=a/1000;
			int f=int(a);
			digit4=f%10;
			int part= int(digit1);
			if(digit1==1)
			{
				masa=0;
			}
			if((digit1==2) || (digit1==3))
			{
				masa=510.9989*10E-6;
			}
			if((digit1==5) || (digit1==6))
			{
				masa=105.6583*10E-3;
			}
			if(digit1==7)
			{
				masa=134.9766*10E-3;
			}
			if((digit1==8) || (digit1==9))
			{
				masa=139.57018*10E-3;
			}
			energia=sqrt((masa*masa)+(px*px)+(py*py)+(pz*pz));
			norma=sqrt((px*px)+(py*py)+(pz*pz));
			theta_rad=acos(pz/norma);
			theta_deg=theta_rad*(180/M_PI);
			char (flag2);
			salida<<part<<" "<<digit4<<" "<<px<<" "<<py<<" "<<pz<<" "<<x<<" "<<y<<" "<<t<<" "<<energia<<" "<<theta_rad<<" "<<theta_deg<<endl;
			flag1++;
		}	
	
	}
	
	salida.close();
	cout<<"PROCESO TERMINADO"<<endl;
	
	return 0;


}
