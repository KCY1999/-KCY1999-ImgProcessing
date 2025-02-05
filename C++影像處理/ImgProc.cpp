//---------------------------------------------------------------------------

#pragma hdrstop

#include "ImgProc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

int compare(const void *a, const void *b)
{
	int aa = *(int *)a;
	int bb = *(int *)b;

	if(aa<bb) return -1;
	if(aa>bb) return 1;

	else
		return 0;
}

int min(int  a,int b)
{
	if(a>=b)
		return b;
	else
		return a;
}
int max(int  a,int b)
{
	if(a>=b)
		return a;
	else
		return b;
}

Graphics::TBitmap *LoadImage(AnsiString Str)
{
	AnsiString ext;
	Graphics::TBitmap *Pic;
	TJPEGImage *Jpeg = new TJPEGImage;

	Pic=new Graphics::TBitmap();

	ext = ExtractFileExt(Str).LowerCase();

	if(ext==".bmp")
	{
		Pic->LoadFromFile(Str);
	}
	else
	{
		Jpeg->LoadFromFile(Str);
		Pic->Assign(Jpeg);
		delete Jpeg;
	}
	return Pic;
}
//------------------------------------------------------------------------------
void GetRGB(Graphics::TBitmap *Bmp,int x, int y, int *r, int *g, int *b)
{
	Byte *ptr;
	ptr=(Byte *)Bmp->ScanLine[y];
	int x1;
	x1=(x<<1)+x;

	*r=ptr[x1+2];
	*g=ptr[x1+1];
	*b=ptr[x1];
}
//------------------------------------------------------------------------------
void ToGray(Graphics::TBitmap *Source, Graphics::TBitmap *Target)
{
	Byte *s_ptr, *t_ptr;
	int r, g, b;
	int width, height;
	int gray;

	width=Source->Width*3;
	height=Source->Height;
	for(int y=0;y<height;y++)
	{
		s_ptr=(Byte *)Source->ScanLine[y];
		t_ptr=(Byte *)Target->ScanLine[y];

		for(int x=0;x<width;x+=3)
		{
			r=s_ptr[x+2];
			g=s_ptr[x+1];
			b=s_ptr[x];

			gray = b*0.114+g*0.587+r*0.299;

			t_ptr[x+2]=gray;
			t_ptr[x+1]=gray;
			t_ptr[x]=gray;
		}
	}
}
//-----------------------------------------------------------------------------
void RGBToHSV(int r, int g, int b, int *ch, int *cs, int *cv)
{
  BYTE *ptr;
  float temp,mi,delta;
  int inter,c;
  float h,s,v;

  mi=min(min(r,g),b);
  v=max(max(r,g),b);
  delta=v-mi;

  if(delta==0)
	h=0;
  else
    {
      if(r==v)
        h = 60.0 * (g - b) / delta;
	  if(g==v)
		h = 120.0 + 60.0 * (b - r) / delta;
      if(b==v)
        h = 240.0 + 60.0 * (r - g) / delta;
      if(h<0.0)
		h = h + 360.0;
	}

  if(v==0.0)
    s=0;
  else
    s=delta/v;

  s*=255.0;

  *ch=h;
  *cs=s;
  *cv=v;
}
//-----------------------------------------------------------------------------
void ToBinary_B(Graphics::TBitmap *Source, Graphics::TBitmap *Target,int th)
{
	Byte *s_ptr, *t_ptr;
	int r, g, b;
	int width, height;

	width=Source->Width*3;
	height=Source->Height;

	for(int y=0;y<height;y++)
	{
		s_ptr=(Byte *)Source->ScanLine[y];
		t_ptr=(Byte *)Target->ScanLine[y];

		for(int x=0;x<width;x+=3)
		{
			b=s_ptr[x];

			if(b >= th)
			{

				t_ptr[x+2]=BINARY_FOREGROUND;
				t_ptr[x+1]=BINARY_FOREGROUND;
				t_ptr[x]=BINARY_FOREGROUND;
			}
			else
			{
				t_ptr[x+2]=BINARY_BACKGROUND;
				t_ptr[x+1]=BINARY_BACKGROUND;
				t_ptr[x]=BINARY_BACKGROUND;
			}
		}
	}
}
//------------------------------------------------------------------------------
void ToBinary_S(Graphics::TBitmap *Source, Graphics::TBitmap *Target,int th)
{
	Byte *s_ptr, *t_ptr;
	int r, g, b;
	int width, height;

	width=Source->Width*3;
	height=Source->Height;

	for(int y=0;y<height;y++)
	{
		s_ptr=(Byte *)Source->ScanLine[y];
		t_ptr=(Byte *)Target->ScanLine[y];

		for(int x=0;x<width;x+=3)
		{
			b=s_ptr[x];

			if(b >= th)
			{
				t_ptr[x+2]=BINARY_BACKGROUND;
				t_ptr[x+1]=BINARY_BACKGROUND;
				t_ptr[x]=BINARY_BACKGROUND;
			}
			else
			{
				t_ptr[x+2]=BINARY_FOREGROUND;
				t_ptr[x+1]=BINARY_FOREGROUND;
				t_ptr[x]=BINARY_FOREGROUND;
			}
		}
	}
}
//------------------------------------------------------------------------------
void MeanFilter(Graphics::TBitmap *Source, Graphics::TBitmap *Target)
{
	Byte *sptr1, *sptr2, *sptr3, *tptr;
	int avg;
	int width, height;

	width=(Source->Width-2)*3;
	height=Source->Height-2;

	for(int y=0;y<height;y++)
	{
		sptr1=(Byte *)Source->ScanLine[y];
		sptr2=(Byte *)Source->ScanLine[y+1];
		sptr3=(Byte *)Source->ScanLine[y+2];
		tptr=(Byte *)Target->ScanLine[y+1];

		for(int x=0;x<width;x+=3)
		{
			avg=(sptr1[x]+sptr1[x+3]+sptr1[x+6]+
				 sptr2[x]+sptr2[x+3]+sptr2[x+6]+
				 sptr3[x]+sptr3[x+3]+sptr3[x+6])/9;

			tptr[x+3]=avg;
			tptr[x+4]=avg;
			tptr[x+5]=avg;
		}
	}
}
//------------------------------------------------------------------------------
void MedianFilter(Graphics::TBitmap *Source, Graphics::TBitmap *Target)
{
	Byte *sptr1, *sptr2, *sptr3, *tptr;
	int data[9];
	int width, height;

	width=(Source->Width-2)*3;
	height=Source->Height-2;

	for(int y=0;y<height;y++)
	{
		sptr1=(Byte *)Source->ScanLine[y];
		sptr2=(Byte *)Source->ScanLine[y+1];
		sptr3=(Byte *)Source->ScanLine[y+2];
		tptr=(Byte *)Target->ScanLine[y+1];

		for(int x=0;x<width;x+=3)
		{
			data[0]=sptr1[x]; data[1]=sptr1[x+3]; data[2]=sptr1[x+6];
			data[3]=sptr2[x]; data[4]=sptr2[x+3]; data[5]=sptr2[x+6];
			data[6]=sptr3[x]; data[7]=sptr3[x+3]; data[8]=sptr3[x+6];

			qsort(data, 9, sizeof(int), compare);

			tptr[x+3]=data[4];
			tptr[x+4]=data[4];
			tptr[x+5]=data[4];
		}
	}
}
//------------------------------------------------------------------------------
void RGBMean(Graphics::TBitmap *Source, Graphics::TBitmap *Target)
{
	Byte *sptr1,*sptr2,*sptr3,*tptr;
	int w,h;
	int avg1,avg2,avg3;

	w=(Source->Width-2)*3;
	h=Source->Height-2;

	for(int y=0;y<h;y++)
	{
	   sptr1=(Byte *)Source->ScanLine[y];
	   sptr2=(Byte *)Source->ScanLine[y+1];
	   sptr3=(Byte *)Source->ScanLine[y+2];
	   tptr=(Byte *)Target->ScanLine[y+1];

		for(int x=0;x<w;x+=3)
		{
		  avg1=(sptr1[x]+sptr1[x+3]+sptr1[x+6]+
				sptr2[x]+sptr2[x+3]+sptr2[x+6]+
				sptr3[x]+sptr3[x+3]+sptr3[x+6])/9;

		  avg2=(sptr1[x+1]+sptr1[x+4]+sptr1[x+7]+
				sptr2[x+1]+sptr2[x+4]+sptr2[x+7]+
				sptr3[x+1]+sptr3[x+4]+sptr3[x+7])/9;

		  avg3=(sptr1[x+2]+sptr1[x+5]+sptr1[x+8]+
				sptr2[x+2]+sptr2[x+5]+sptr2[x+8]+
				sptr3[x+2]+sptr3[x+5]+sptr3[x+8])/9;

		  tptr[x+3]=avg1;
		  tptr[x+4]=avg2;
		  tptr[x+5]=avg3;
		}
	}
}
//------------------------------------------------------------------------------
void RGBMedian(Graphics::TBitmap *Source, Graphics::TBitmap *Target)
{
	Byte *sptr1,*sptr2,*sptr3,*tptr;
	int w,h;
	int data1[9],data2[9],data3[9];

	w=(Source->Width-2)*3;
	h=Source->Height-2;

	for(int y=0;y<h;y++)
	{
		sptr1=(Byte *)Source->ScanLine[y];
		sptr2=(Byte *)Source->ScanLine[y+1];
		sptr3=(Byte *)Source->ScanLine[y+2];
		tptr=(Byte *)Target->ScanLine[y+1];

		for(int x=0;x<w;x+=3)
		{
			data1[0]=sptr1[x]; data1[1]=sptr1[x+3]; data1[2]=sptr1[x+6];
			data1[3]=sptr2[x]; data1[4]=sptr2[x+3]; data1[5]=sptr2[x+6];
			data1[6]=sptr3[x]; data1[7]=sptr3[x+3]; data1[8]=sptr3[x+6];

			qsort(data1,9,sizeof(int),compare);

			data2[0]=sptr1[x+1]; data2[1]=sptr1[x+4]; data2[2]=sptr1[x+7];
			data2[3]=sptr2[x+1]; data2[4]=sptr2[x+4]; data2[5]=sptr2[x+7];
			data2[6]=sptr3[x+1]; data2[7]=sptr3[x+4]; data2[8]=sptr3[x+7];

			qsort(data2,9,sizeof(int),compare);

			data3[0]=sptr1[x+2]; data3[1]=sptr1[x+5]; data3[2]=sptr1[x+8];
			data3[3]=sptr2[x+2]; data3[4]=sptr2[x+5]; data3[5]=sptr2[x+8];
			data3[6]=sptr3[x+2]; data3[7]=sptr3[x+5]; data3[8]=sptr3[x+8];

			qsort(data3,9,sizeof(int),compare);

			tptr[x+3]=data1[4];
			tptr[x+4]=data2[4];
			tptr[x+5]=data3[4];
		}
	}
}
//------------------------------------------------------------------------------
void SharpenFilter(Graphics::TBitmap *Source, Graphics::TBitmap *Target)
{
	float Mask[9] = {-1.0, -1.0, -1.0,
					 -1.0,  8.0, -1.0,
					 -1.0, -1.0, -1.0
					};
	HeighBoostFilter(Source, Target, Mask);
}
//------------------------------------------------------------------------------
void HeighBoostFilter(Graphics::TBitmap *Source, Graphics::TBitmap *Target, float *Mask)
{
	float value;
	int height, width;
	Byte *spt0, *spt1, *spt2, *tptr;

	width=(Source->Width-2)*3;
	height=Source->Height-2;

	for(int y=0;y<height;y++)
	{
		spt0=(Byte *)Source->ScanLine[y];
		spt1=(Byte *)Source->ScanLine[y+1];
		spt2=(Byte *)Source->ScanLine[y+2];
		tptr=(Byte *)Target->ScanLine[y+1];

		for(int x=0;x<width;x+=3)
		{
			value=(spt0[x]*Mask[0]+spt0[x+3]*Mask[1]+spt0[x+6]*Mask[2]+
				   spt1[x]*Mask[3]+spt1[x+3]*Mask[4]+spt1[x+6]*Mask[5]+
				   spt2[x]*Mask[6]+spt2[x+3]*Mask[7]+spt2[x+6]*Mask[8])/9;

			tptr[x+3]=spt1[x+3]+value;
			tptr[x+4]=spt1[x+4]+value;
			tptr[x+5]=spt1[x+5]+value;
		}
	}
}
//------------------------------------------------------------------------------
void RGBSharpenFilter(Graphics::TBitmap *Source, Graphics::TBitmap *Target)
{
	float Mask[9] = {-1.0, -1.0, -1.0,
					 -1.0,  8.0, -1.0,
					 -1.0, -1.0, -1.0
					};
	RGBBoostFilter(Source, Target, Mask);
}
//------------------------------------------------------------------------------
void RGBBoostFilter(Graphics::TBitmap *Source, Graphics::TBitmap *Target, float *Mask)
{
	float v1,v2,v3;
	int height, width;
	Byte *spt0, *spt1, *spt2, *tptr;

	width=(Source->Width-2)*3;
	height=Source->Height-2;

	for(int y=0;y<height;y++)
	{
		spt0=(Byte *)Source->ScanLine[y];
		spt1=(Byte *)Source->ScanLine[y+1];
		spt2=(Byte *)Source->ScanLine[y+2];
		tptr=(Byte *)Target->ScanLine[y+1];

		for(int x=0;x<width;x+=3)
		{
			v1=( spt0[x]*Mask[0]+spt0[x+3]*Mask[1]+spt0[x+6]*Mask[2]+
				 spt1[x]*Mask[3]+spt1[x+3]*Mask[4]+spt1[x+6]*Mask[5]+
				 spt2[x]*Mask[6]+spt2[x+3]*Mask[7]+spt2[x+6]*Mask[8])/9;

			v2=( spt0[x+1]*Mask[0]+spt0[x+4]*Mask[1]+spt0[x+7]*Mask[2]+
				 spt1[x+1]*Mask[3]+spt1[x+4]*Mask[4]+spt1[x+7]*Mask[5]+
				 spt2[x+1]*Mask[6]+spt2[x+4]*Mask[7]+spt2[x+7]*Mask[8])/9;

			v3=( spt0[x+2]*Mask[0]+spt0[x+5]*Mask[1]+spt0[x+8]*Mask[2]+
				 spt1[x+2]*Mask[3]+spt1[x+5]*Mask[4]+spt1[x+8]*Mask[5]+
				 spt2[x+2]*Mask[6]+spt2[x+5]*Mask[7]+spt2[x+8]*Mask[8])/9;


			tptr[x+3]=spt1[x+3]+v1;
			tptr[x+4]=spt1[x+4]+v2;
			tptr[x+5]=spt1[x+5]+v3;
        }
	}

}
//------------------------------------------------------------------------------
void Ddifference(Graphics::TBitmap *Source1,Graphics::TBitmap *Source2,Graphics::TBitmap *Target)
{
	Byte *sptr1,*sptr2,*tptr;
	int dif;
	int v=50;

	int w1,h1,w2,h2;

	w1=Source1->Width*3;
	h1=Source1->Height;

	for(int y=0;y<h1;y++)
	{
		sptr1=(Byte *)Source1->ScanLine[y];
		sptr2=(Byte *)Source2->ScanLine[y];
		tptr=(Byte *)Target->ScanLine[y];

		for(int x=0;x<w1;x++)
		{
			dif=sptr1[x]-sptr2[x];

			if(dif < v && dif > -v)
			{
				tptr[x]=255;
			}
			else
			{
				tptr[x]=0;
			}
		}
	}
}
//------------------------------------------------------------------------------
void H_Projection(Graphics::TBitmap *Bmp,int H[])
{
	Byte *ptr;
	int width,height;

	width=Bmp->Width*3;
	height=Bmp->Height;

	for(int y=0;y<height;y++)
	{
		ptr=(Byte*)Bmp->ScanLine[y];
		for(int x=0;x<width;x+=3)
		{
			if(ptr[x]==BINARY_FOREGROUND)
			H[y]++;
		}
	}
}
//------------------------------------------------------------------------------
void DrawHProjection(int H[],TImage *Img,int Height , float scale)
{
	int Count;
	Img->Canvas->Pen->Color=clBlack;

	for(int i=0;i<Height;i++)
	{
		Count=(float)H[i]/scale;
		Img->Canvas->MoveTo(Img->Width-Count,i);
		Img->Canvas->LineTo(Img->Width,i);
	}
}
//------------------------------------------------------------------------------
void V_Projection(Graphics::TBitmap *Bmp,int V[])
{
	Byte *ptr;
	int width,height;

	width=Bmp->Width*3;
	height=Bmp->Height;

	for(int y=0;y<height;y++)
	{
		ptr=(Byte *)Bmp->ScanLine[y];

		for(int x=0;x<width;x+=3)
		{
			if(ptr[x]==BINARY_FOREGROUND)
			V[x/3]++;
		}
	}
}
//------------------------------------------------------------------------------
void DrawVProjection(int V[],TImage *Img,int Height,float scale)
{
	int Count;
	Img->Canvas->Pen->Color=clBlack;

	for(int i=0;i<Height;i++)
	{
		Count=(float)V[i]/scale;
		Img->Canvas->MoveTo(i,Img->Height-Count);
		Img->Canvas->LineTo(i,Img->Height);
	}
}
//------------------------------------------------------------------------------
void SobelFilter(Graphics::TBitmap *Source, Graphics::TBitmap *Target)
{
  int Mask1[9] = { -1.0 , 0.0 ,  1.0 ,
                   -2.0 , 0.0 ,  2.0 ,
                   -1.0 , 0.0 ,  1.0
                 }; // detect the horizontal edges
  int Mask2[9] = {  1.0 , 2.0 , 1.0 ,
                    0.0 , 0.0 , 0.0 ,
                   -1.0 ,-2.0 ,-1.0
                 }; // detect the vertical edges

  Converlution2Times(Source,Target,Mask1,Mask2,4);

}

//------------------------------------------------------------------------------
void Converlution2Times(Graphics::TBitmap *Source,Graphics::TBitmap *Target,
                        int *Mask1, int *Mask2,int factor)
{
	int value;
	int height,width;
	Byte *spt0,*tptr,*spt1,*spt2;

	width=(Source->Width-2)*3;
	height=Source->Height-2;

	for(int y=0;y<height;y++)
	{
		spt0 = (Byte *)Source->ScanLine[y];
        spt1 = (Byte *)Source->ScanLine[y+1];
        spt2 = (Byte *)Source->ScanLine[y+2];
        tptr = (Byte *)Target->ScanLine[y+1];
        for(int x=0;x<width;x+=3)
		{
			value=abs(spt0[x]*Mask1[0]+spt0[x+3]*Mask1[1]+spt0[x+6]*Mask1[2]+
					  spt1[x]*Mask1[3]+spt1[x+3]*Mask1[4]+spt1[x+6]*Mask1[5]+
					  spt2[x]*Mask1[6]+spt2[x+3]*Mask1[7]+spt2[x+6]*Mask1[8]);

			value/=factor;

			tptr[x+3]=value;
			tptr[x+4]=value;
			tptr[x+5]=value;
		}
	}
//------------------------------------------------------------------------------
	for(int y=0;y<height;y++)
	{
		spt0 = (Byte *)Source->ScanLine[y];
		spt1 = (Byte *)Source->ScanLine[y+1];
		spt2 = (Byte *)Source->ScanLine[y+2];
		tptr = (Byte *)Target->ScanLine[y+1];

		for(int x=0;x<width;x+=3)
		{
			value=abs(spt0[x]*Mask2[0]+spt0[x+3]*Mask2[1]+spt0[x+6]*Mask2[2]+
					  spt1[x]*Mask2[3]+spt1[x+3]*Mask2[4]+spt1[x+6]*Mask2[5]+
					  spt2[x]*Mask2[6]+spt2[x+3]*Mask2[7]+spt2[x+6]*Mask2[8]);

			value/=factor;

			   // 新的程式碼： 白色為背景
			if(tptr[x+3]+value>255)
			{
				tptr[x+3]=BINARY_FOREGROUND;
				tptr[x+4]=BINARY_FOREGROUND;
				tptr[x+5]=BINARY_FOREGROUND;
			}

			if(tptr[x+3]+value<0)
			{
				tptr[x+3]=BINARY_BACKGROUND;
				tptr[x+4]=BINARY_BACKGROUND;
				tptr[x+5]=BINARY_BACKGROUND;
			}
		   else
		   {
				tptr[x+3]+=value;
				tptr[x+4]+=value;
				tptr[x+5]+=value;

				tptr[x+3]=255-tptr[x+3];
				tptr[x+4]=255-tptr[x+4];
				tptr[x+5]=255-tptr[x+5];
		   }
		}
	}
}
