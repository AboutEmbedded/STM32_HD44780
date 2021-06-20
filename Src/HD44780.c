#include "stm32f1xx_hal.h"
#include "HD44780.h"
#include "DWT_delay.h"

#define Delay_us 150


//массивы для русификации дисплея
//рисовалка символов https://codepen.io/ygricks/pen/RPPvMg?editors=101
//https://jsfiddle.net/Ristee/gxbgfLc6/3/
uint8_t Arr_Be[]={ 0x1F, 0x10, 0x10, 0x1F, 0x11, 0x11, 0x1F, 0x00}; 	//Б
uint8_t Arr_Ge[]={ 0x1F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00}; 	//Г
uint8_t Arr_De[]={ 0x0E, 0x0A, 0x0A, 0x0A, 0x1B, 0x11, 0x11, 0x00}; 	//Д
uint8_t Arr_Je[]={ 0x15, 0x15, 0x15, 0x0E, 0x15, 0x15, 0x15, 0x00}; 	//Ж
uint8_t Arr_I[]= { 0x11, 0x11, 0x11, 0x13, 0x15, 0x19, 0x11, 0x00}; 	//И 
uint8_t Arr_El[]={ 0x07, 0x09, 0x09, 0x09, 0x09, 0x09, 0x11, 0x00}; 	//Л
uint8_t Arr_Pe[]={ 0x1F, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00}; 	//П
uint8_t Arr_Ef[]={ 0x0E, 0x15, 0x15, 0x15, 0x0e, 0x04, 0x04, 0x00}; 	//Ф 
uint8_t Arr_Ce[]={ 0x12, 0x12, 0x12, 0x12, 0x12, 0x1F, 0x01, 0x00}; 	//Ц 
uint8_t Arr_Ch[]={ 0x11, 0x11, 0x11, 0x0f, 0x01, 0x01, 0x01, 0x00}; 	//Ч
uint8_t Arr_Sh[]={ 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x1F, 0x00}; 	//Ш
uint8_t Arr_Shch[]={0x15, 0x15, 0x15, 0x15, 0x15, 0x1F, 0x01, 0x00};  //Щ
uint8_t Arr_tZn[]={ 0x18, 0x08, 0x08, 0x0F, 0x09, 0x09, 0x0F, 0x00}; 	//Ъ
uint8_t Arr_Yi[]={ 0x11, 0x11, 0x11,  0x1D, 0x15, 0x15, 0x1D, 0x00}; 	//Ы
uint8_t Arr_mZn[]={0x10, 0x10, 0x10, 0x1e, 0x12, 0x12, 0x1e, 0x00}; 	//Ь
uint8_t Arr_Ee[]={ 0x0e, 0x11, 0x01, 0x0f, 0x01, 0x11, 0x0e, 0x00,}; 	//Э +
uint8_t Arr_Ya[]={0x0F, 0x11, 0x11,  0x0F, 0x05, 0x09, 0x11, 0x00}; 	//Я


//батарейка
uint8_t Bat_low[]=	{ 0xe,0x11,0x11,0x11,0x11,0x1f,0x1f,0x1f};
uint8_t Bat_medium[]={	0xe,0x11,0x11,0x1f,0x1f,0x1f,0x1f,0x1f};
uint8_t Bat_High[]={	0xe,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f};

void HD4w_CLK_impulse ()
{
	DWT_delay_us(10);
	E_set;
	DWT_delay_us(10);
	E_reset;
	DWT_delay_us(10);
}

//Запись полубайта в 4-х проводном режиме
void HD4w_WriteHalfByte (uint8_t Data_byte, uint8_t Data_command)
{
	if(Data_command==HD_Data) RS_set;	else RS_reset;	
		DWT_delay_us(Delay_us);
	//выводим младшие 4 бита
	if((Data_byte & 0x08)>0) D7_set;	else D7_reset;
	if((Data_byte & 0x04)>0) D6_set;	else D6_reset;
	if((Data_byte & 0x02)>0) D5_set;	else D5_reset;
	if((Data_byte & 0x01)>0) D4_set;	else D4_reset;
	HD4w_CLK_impulse();
}

//Запись байта в 4-х проводном режиме
void HD4w_WriteByte (uint8_t Data_byte, uint8_t Data_command)
{
	if(Data_command==HD_Data) RS_set;	else RS_reset;
	DWT_delay_us(10);	
	//выводим по старшие 4 бита
	if((Data_byte & 0x80)>0) D7_set;	else D7_reset;
	if((Data_byte & 0x40)>0) D6_set;	else D6_reset;
	if((Data_byte & 0x20)>0) D5_set;	else D5_reset;
	if((Data_byte & 0x10)>0) D4_set;	else D4_reset;
	HD4w_CLK_impulse();

	DWT_delay_us(Delay_us);
	//выводим младшие 4 бита
	if((Data_byte & 0x08)>0) D7_set;	else D7_reset;
	if((Data_byte & 0x04)>0) D6_set;	else D6_reset;
	if((Data_byte & 0x02)>0) D5_set;	else D5_reset;
	if((Data_byte & 0x01)>0) D4_set;	else D4_reset;
	HD4w_CLK_impulse();
	
	DWT_delay_us(Delay_us);

}
//очистка дисплея
void HD4w_Clear()
{
	HD4w_WriteByte(0x01, HD_Command);
	HAL_Delay(2);
}


//инициализация дисплея
void HD4w_init()
{
	HAL_Delay(60);
	
		HD4w_WriteHalfByte(0x03, HD_Command); //8 битный режим, 2 линии, 5x8 формат
				HAL_Delay(4);
		HD4w_WriteHalfByte(0x03, HD_Command); //8 битный режим, 2 линии, 5x8 формат
				HAL_Delay(4);
		HD4w_WriteHalfByte(0x03, HD_Command); //8 битный режим, 2 линии, 5x8 формат
				DWT_delay_us(300);
		HD4w_WriteHalfByte(0x02, HD_Command); //4 битный режим
				DWT_delay_us(Delay_us);
				
		HD4w_WriteByte(0x28, HD_Command); 	//вкл. 2-х строчный дисплей, символы 5х8 точек
				DWT_delay_us(Delay_us);;
	HD4w_WriteByte(0x0D, HD_Command); //вкл. отображение, мигающий курсор/знакоместо (0x0c курсоры отключены)
				DWT_delay_us(Delay_us);
		HD4w_Clear();
		HD4w_WriteByte(0x06, HD_Command); //инкремент курсора после ввода символа
				DWT_delay_us(Delay_us);
//		HD4w_WriteByte(0x02, HD_Command); //адрессация в 0

}


//ВЫВОД ИНФОРМАЦИИ НА ДИСПЛЕЙ

//Запись символа по адресу
void HD4w_WriteSymbol(uint8_t pX, uint8_t pY, uint8_t Data_byte)
{
	HD4w_WriteByte((((0x40*pY)|pX)|0x80), HD_Command); //установка курсора в требуемый сегмент 1-й строки
	HD4w_WriteByte(Data_byte, HD_Data);
}

//запись строки по адресу
void HD4w_WriteString (uint8_t pX, uint8_t pY, uint8_t Data_Arr[])
{
	uint8_t i=0;
	HD4w_WriteSymbol(pX, pY, Data_Arr[i]);
	i++;
  while(Data_Arr[i]!=0)
	{
    HD4w_WriteByte(Data_Arr[i], HD_Data);
		i++;
	}
}


//запись цифр 0-999 по адресу
void HD4w_WriteNumeric (uint8_t pX, uint8_t pY, uint16_t Data_3num)
{
	uint8_t Arr[3];
	
	Arr[0]=Data_3num/100; 				//сотни
	Arr[1]=(Data_3num%100)/10; //десятки
	Arr[2]=Data_3num%10; //единицы
	
	for(uint8_t i=0;i<sizeof(Arr);i++) Arr[i]+=0x30;
	
	uint8_t i=0;
	HD4w_WriteSymbol(pX, pY, Arr[i]);
	i++;
  while(Arr[i+2]!=0) //не очень понятно почему так (+2), но иначе проблема при отображении (в паре с hex например)
	{
    HD4w_WriteByte(Arr[i], HD_Data);
		i++;
	}
}


//конвертация числа в hex
uint8_t DecToHex_ASCII (uint8_t DataDec)
{
	uint8_t DataHex=0;
	if(DataDec<10) DataHex=DataDec+0x30;
	else
	{
		if(DataDec==10) DataHex='A';
		if(DataDec==11) DataHex='B';
		if(DataDec==12) DataHex='C';
		if(DataDec==13) DataHex='D';
		if(DataDec==14) DataHex='E';
		if(DataDec==15) DataHex='F';
	}
	return (DataHex);
}

//запись hex по адресу
void HD4w_WriteHex (uint8_t pX, uint8_t pY, uint8_t Data_Byte)
{

	uint8_t Arr[4];
	Arr[0]='0';
	Arr[1]='x';
	
	Arr[2]=Data_Byte/16;
	Arr[3]=Data_Byte-(16*Arr[2]);
	
	Arr[2]=DecToHex_ASCII(Arr[2]);
	Arr[3]=DecToHex_ASCII(Arr[3]);
	uint8_t i=0;	
	HD4w_WriteSymbol(pX, pY, Arr[i]);
	i++;
  while(Arr[i+1]!=0)   //не очень понятно почему так (+1), но иначе проблема при отображении (в паре с dec например)
	{
    HD4w_WriteByte(Arr[i], HD_Data);
		i++;
	}
}


void HD4w_WriteMy_symbol_in_buf(uint8_t Num, uint8_t ArrRus[])
{
	HAL_Delay(10);
	HD4w_WriteByte(0x40|(Num*8), HD_Command); //адресация в 1 сегмент CGRAM 
	for(uint8_t i=0;i<8;i++)
	{
    HD4w_WriteByte(ArrRus[i], HD_Data);
	}	
}

//USER-MEMORY и Русские буквы
void HD4w_user_memory_init ()
{
		HD4w_WriteMy_symbol_in_buf(1, Arr_Pe); //П
		HD4w_WriteMy_symbol_in_buf(2, Arr_De); //Д
		HD4w_WriteMy_symbol_in_buf(3, Arr_I);  //И
//	HD4w_WriteMy_symbol_in_buf(4, Arr_Pe);
//	HD4w_WriteMy_symbol_in_buf(5, Arr_De);
//	HD4w_WriteMy_symbol_in_buf(6, Arr_I);
//	HD4w_WriteMy_symbol_in_buf(7, Arr_Pe);
}

//Функция перевода(подбора) аналогов русским символам + символы из памяти для пользовательских символов
void HD4w_Russian_to_Ascii(uint8_t Sym_En[])
{
		uint8_t i=0;
	  while(Sym_En[i]!=0)   //не очень понятно почему так (+1), но иначе проблема при отображении (в паре с dec например)
	{
			if(Sym_En[i]==0xc0) Sym_En[i]='A'; //win1251 (Cyrillic)
			if(Sym_En[i]==0xc2) Sym_En[i]='B';
			if(Sym_En[i]==0xd1) Sym_En[i]='C';
			if(Sym_En[i]==0xc5) Sym_En[i]='E';
			if(Sym_En[i]==0xcd) Sym_En[i]='H';
			if(Sym_En[i]==0xca) Sym_En[i]='K';
			if(Sym_En[i]==0xcc) Sym_En[i]='M';
			if(Sym_En[i]==0xce) Sym_En[i]='O';
			if(Sym_En[i]==0xd0) Sym_En[i]='P';
			if(Sym_En[i]==0xd2) Sym_En[i]='T';
			if(Sym_En[i]==0xd5) Sym_En[i]='X';
			if(Sym_En[i]==0xd3) Sym_En[i]='Y';
			if(Sym_En[i]==0xc7) Sym_En[i]='3';
		
			//user_symbols
			/*ТУТ НУЖНО УКАЗАТЬ НОМЕР СИМВОЛА В user-memory HD44780*/
			if(Sym_En[i]==0xcf) Sym_En[i]=0x01; //П
			if(Sym_En[i]==0xc4) Sym_En[i]=0x02; //Д
			if(Sym_En[i]==0xc8) Sym_En[i]=0x03; //И
			i++;
	}		

}



//ДЛЯ БАТАРЕЙКИ
void HD4w_Battery_init()
{
	HD4w_WriteMy_symbol_in_buf(0,Bat_low);
	HD4w_WriteMy_symbol_in_buf(1,Bat_medium);
	HD4w_WriteMy_symbol_in_buf(2,Bat_High);
}

void HD4w_Battery_view (uint8_t PlaceX,uint8_t PlaceY,uint8_t Bat_Lvl_percent)
{
	if(Bat_Lvl_percent<30)	HD4w_WriteSymbol(PlaceX,PlaceY,0x00); //low
	if((Bat_Lvl_percent>=30) && (Bat_Lvl_percent<60))	HD4w_WriteSymbol(PlaceX,PlaceY,0x01); //medium
	if(Bat_Lvl_percent>=60)	HD4w_WriteSymbol(PlaceX,PlaceY,0x02); //medium
}



//ТЕСТОВЫЕ ФУНКЦИИ (тест дисплеев)

//тест таблицы символов дисплея
void HD4wTableTest ()
{
			for(uint8_t i=0;i<=0xff;i++)
    {
			HD4w_WriteString (0, 0,((uint8_t*) "Symbol: "));
			HD4w_WriteString (10, 0,((uint8_t*) "'"));
			HD4w_WriteSymbol(11, 0, i);		//функция записи символа по адресу
			HD4w_WriteString (12, 0,((uint8_t*) "'"));
			
			HD4w_WriteString (0, 1,((uint8_t*) "Code: "));
			HD4w_WriteNumeric (8, 1, i);
			HD4w_WriteHex(12,1,i);
			HAL_Delay(500);
    }
}


void Test_user_symbols () //работает до перезапуска м/к (память символов энергозависима)
{
	HD4w_WriteSymbol(0,0,0x00);
	for(uint8_t i=1;i<16;i++)
	{
    HD4w_WriteByte(i, HD_Data);
	}
		HD4w_WriteSymbol(0,1,'T');
}










