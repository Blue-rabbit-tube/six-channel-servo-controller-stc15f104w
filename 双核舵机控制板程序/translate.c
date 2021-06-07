int translate(unsigned char mdsb)
{
unsigned int mdsbint;
if(mdsb=='0')
   mdsbint=0;
if(mdsb=='1')
   mdsbint=1;
if(mdsb=='2')
   mdsbint=2;
if(mdsb=='3')
   mdsbint=3;
if(mdsb=='4')
   mdsbint=4;
if(mdsb=='5')
   mdsbint=5;
if(mdsb=='6')
   mdsbint=6;
if(mdsb=='7')
   mdsbint=7;
if(mdsb=='8')
   mdsbint=8;
if(mdsb=='9')
   mdsbint=9;
return mdsbint;
}