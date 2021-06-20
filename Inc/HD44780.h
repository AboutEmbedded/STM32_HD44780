/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HD44780_H__
#define __HD44780_H__

//делает выбор команда/данные
#define RS_set 			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET)
#define RS_reset 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET)

//RW д.б. LOW
#define RW_set 			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET)
#define RW_reset 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET)

//стробирующий импульс
#define E_set 			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET)
#define E_reset 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET)

//биты данных
#define D4_set 			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)
#define D4_reset 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)

#define D5_set 			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)
#define D5_reset 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)

#define D6_set 			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET)
#define D6_reset 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET)

#define D7_set 			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET)
#define D7_reset 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET)

#define HD_Command 0x00
#define HD_Data 0xff

void HD4w_init(void); 																								//функция инициализации дисплея
void HD4w_user_memory_init (void);																		//функция инициализации и записи символов в пользовательскую память
void HD4w_Clear(void);																								//функция очистки дисплея

void HD4w_WriteSymbol_char(unsigned char Data_byte);									//функция записи символа (удобно использовать с инкрементом)
void HD4w_WriteSymbol(uint8_t pX, uint8_t pY, uint8_t Data_byte);			//функция записи символа по адресу
void HD4w_WriteString (uint8_t pX, uint8_t pY, uint8_t Data_Arr[]); 	//функция записи строки
void HD4w_WriteNumeric (uint8_t pX, uint8_t pY, uint16_t Data_3num); 	//функция записи числа 0-999

void HD4w_Russian_to_Ascii(uint8_t Sym_En[]); 												//функция конвертации англоязычного текста в русскоязычный


//Важно нельзя дублировать символы батареи и другие в пользовательской памяти!
//Батарейка
void HD4w_Battery_init(void);																													//Запись символов батареи
void HD4w_Battery_view (uint8_t PlaceX,uint8_t PlaceY,uint8_t Bat_Lvl_percent);				//Отображение батарейки


//Функции для теста дисплея
void HD4wTableTest (void); 				//выводит всю таблицу символов из памяти дисплея
void Test_user_symbols (void); 		//выводит все записанные пользователем символы (дублируются 1-7 и 9-15)


//массивы для русификации дисплея
extern uint8_t Arr_Be[]; 		//Б
extern uint8_t Arr_Ge[]; 		//Г
extern uint8_t Arr_De[]; 		//Д
extern uint8_t Arr_Je[]; 		//Ж
extern uint8_t Arr_I[]; 		//И
extern uint8_t Arr_El[]; 		//Л
extern uint8_t Arr_Pe[]; 		//П
extern uint8_t Arr_Ef[]; 		//Ф
extern uint8_t Arr_Ce[]; 		//Ц
extern uint8_t Arr_Ch[]; 		//Ч
extern uint8_t Arr_Sh[]; 		//Ш
extern uint8_t Arr_Shch[]; 	//Щ
extern uint8_t Arr_tZn[]; 	//Ъ
extern uint8_t Arr_Yi[]; 		//Ы
extern uint8_t Arr_mZn[]; 	//Ь
extern uint8_t Arr_Ee[]; 		//Э
extern uint8_t Arr_Ya[]; 		//Я

#endif /* __HD44780_H__ */
