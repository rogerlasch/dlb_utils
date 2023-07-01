
//dlb_keyboard_h
//Esta biblioteca faz parte da dlb_game.
//Escrito por R�ger: rogerlasch@gmail.com em 2019, e atualizada em 24 de dezembro de 2020.
//Fun��es para a verifica��o de teclas no windows.
//Note que � preciso tratar as mensagens WM_KEYDOWN e WM_KEYUP para que essa biblioteca funcione bem.
#ifndef DLB_KEYBOARD_H
#define DLB_KEYBOARD_H

namespace dlb
{
enum keyboard_state
{
KEY_UP=0,//A tecla est� para cima.
KEY_PRESSED=1,//O usu�rio acabou de pressionar a tecla.
KEY_DOWN=2,//O usu�rio est� mantendo pressionada a tecla.
KEY_BLOCKING=3//N�o usado no momento.
};

bool dlb_key_pressed(int32 key_code);//Verifica se uma tecla foi pressionada. Consulte o arquivo winuser.h para ver as constantes.
bool dlb_key_down(int32 key_code);//Verifica se uma tecla est� sendo mantida pressionada.
bool dlb_key_up(int32 key_code);//Verifica se uma tecla est� para cima.
void dlb_keyboard_setkeystate(int32 key_code);//Define o estado de uma tecla. Chame esta fun��o para tratar a mensagem WM_KEYDOWN passando o par�metro wParam.
void dlb_keyboard_release_key(int32 key_code);//Libera uma tecla. Chame esta fun��o ao tratar a mensagem WM_KEYUP com o par�metro wParam.
void dlb_keyboard_reset();//Reseta o teclado, todas teclas para cima.
bool dlb_keyboard_isready();//Verifica se o teclado foi inicializado antes do uso. Note que isso � interno da biblioteca. N�o tem nada haver com o teclado f�sico.
}
#endif
