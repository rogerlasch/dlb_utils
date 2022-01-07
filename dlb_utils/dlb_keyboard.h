
//dlb_keyboard_h
//Esta biblioteca faz parte da dlb_game.
//Escrito por Róger: rogerlasch@gmail.com em 2019, e atualizada em 24 de dezembro de 2020.
//Funções para a verificação de teclas no windows.
//Note que é preciso tratar as mensagens WM_KEYDOWN e WM_KEYUP para que essa biblioteca funcione bem.
#ifndef DLB_KEYBOARD_H
#define DLB_KEYBOARD_H

namespace dlb
{
enum keyboard_state
{
KEY_UP=0,//A tecla está para cima.
KEY_PRESSED=1,//O usuário acabou de pressionar a tecla.
KEY_DOWN=2,//O usuário está mantendo pressionada a tecla.
KEY_BLOCKING=3//Não usado no momento.
};

bool dlb_key_pressed(int32 key_code);//Verifica se uma tecla foi pressionada. Consulte o arquivo winuser.h para ver as constantes.
bool dlb_key_down(int32 key_code);//Verifica se uma tecla está sendo mantida pressionada.
bool dlb_key_up(int32 key_code);//Verifica se uma tecla está para cima.
void dlb_keyboard_setkeystate(int32 key_code);//Define o estado de uma tecla. Chame esta função para tratar a mensagem WM_KEYDOWN passando o parâmetro wParam.
void dlb_keyboard_release_key(int32 key_code);//Libera uma tecla. Chame esta função ao tratar a mensagem WM_KEYUP com o parâmetro wParam.
void dlb_keyboard_reset();//Reseta o teclado, todas teclas para cima.
bool dlb_keyboard_isready();//Verifica se o teclado foi inicializado antes do uso. Note que isso é interno da biblioteca. Não tem nada haver com o teclado físico.
}
#endif
