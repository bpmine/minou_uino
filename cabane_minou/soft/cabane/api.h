/*
 * api.h
 *
 *  Created on: 23 déc. 2024
 *      Author: Utilisateur
 */

#ifndef API_HEADER_INCLUDED
#define API_HEADER_INCLUDED

#define C_OFF     0
#define C_RED     1
#define C_GREEN   2
#define C_BLUE    3

extern bool api_strip_getpwr_bas(void);
extern bool api_strip_getpwr_haut(void);
extern bool api_strip_getpwr_tour(void);
extern bool api_strip_getpwr_res(void);
extern bool api_strip_getpwr_serial(void);

extern void api_strip_setpwr_bas(bool en);
extern void api_strip_setpwr_haut(bool en);
extern void api_strip_setpwr_tour(bool en);
extern void api_strip_setpwr_res(bool en);

extern void api_strip_setpwr_serial(bool en);

extern void api_strip_setcol_bas(int col);
extern void api_strip_setcol_haut(int col);
extern void api_strip_setcol_tour(int col);
extern void api_strip_setcol_res(int col);

extern void api_set_mode(int mode);
extern void api_set_aff(int aff);
extern int api_get_mode(void);
extern int api_get_aff(void);


extern void api_get_date_hour(int &day,int &month,int &year,int &hour,int &minute,int &second);
extern void api_get_date_hour(int &day,int &month,int &year,int &hour,int &minute,int &second,int &dow);
extern void api_set_date(int day,int month,int year);
extern void api_set_hour(int hour,int minute,int second);

extern float api_get_temp_bas(void);
extern float api_get_hum_bas(void);
extern float api_get_temp_ext(void);
extern float api_get_hum_ext(void);

#endif
