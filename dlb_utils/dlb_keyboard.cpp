
#include<mutex>
#include<string.h>
#include"dlb_types.h"
#include"dlb_keyboard.h"

using namespace std;
namespace dlb
{
static mutex mtx;//Objeto mutex para evitar corridas de dados em multi threading.
static bool KEYBOARD_STARTED=false;
static uint8 keyboard [256];//Armazena o estado de todas as teclas do teclado.

//Redefine todas teclas do teclado...
void dlb_keyboard_reset()
{
unique_lock<mutex> lck(mtx);//Nenhum outro thread pode acessar ao mesmo tempo esta fun��o.
memset(keyboard, 0, 256);
KEYBOARD_STARTED=true;
}

bool dlb_keyboard_isready()
{
unique_lock<mutex> lck(mtx);
return KEYBOARD_STARTED;
}

//Define o estado de uma tecla.
void dlb_keyboard_setkeystate(int32 key_code)
{
if(key_code>255)
{
return;
}
unique_lock<mutex> lck(mtx);//adquire acesso exclusivo e nenhum outro thread pode auterar nada por enquanto.
switch(keyboard[key_code])
{
//Caso a tecla esteja para cima, ent�o o pr�ximo estado l�gico � de ser pressionada.
case KEY_UP:
{
keyboard[key_code]=KEY_PRESSED;
break;
}
//Caso a tecla acabou de ser pressionada, ent�o o usu�rio est� mantendo a mesma pressionada.
case KEY_PRESSED:
{
keyboard[key_code]=KEY_DOWN;
break;
}
default:
{
return;
}
}
}

//Libera uma tecla do pressionamento.
void dlb_keyboard_release_key(int32 key_code)
{
if(key_code>255)
{
return;
}
unique_lock<mutex> lck(mtx);
keyboard[key_code]=KEY_UP;
}

//Uma tecla est� para cima?
bool dlb_key_up(int32 key_code)
{
if(!dlb_keyboard_isready())
{
dlb_keyboard_reset();
return true;
}
unique_lock<mutex> lck(mtx);
return keyboard[key_code]==KEY_UP;
}

//Foi pressionada?
bool dlb_key_pressed(int32 key_code)
{
if(!dlb_keyboard_isready())
{
dlb_keyboard_reset();
return false;
}
unique_lock<mutex> lck(mtx);
//Se est� pressionada ent�o mude o estado para sendo mantida pressionada. M�gica para fazer o key_down e key_pressed serem distintas entre si.
if(keyboard[key_code]==KEY_PRESSED)
{
keyboard[key_code]=KEY_DOWN;
return true;
}
return false;
}

//Est� sendo mantida pressionada?
bool dlb_key_down(int32 key_code)
{
if(dlb_key_pressed(key_code))
{
return true;
}
unique_lock<mutex> lck(mtx);
if(keyboard[key_code]==KEY_DOWN)
{
return true;
}
return false;
}
}
