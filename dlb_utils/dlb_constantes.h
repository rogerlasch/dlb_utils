

/**
*Algumas constantes usado pela dlb. Este arquivo ainda � vers�o beta e algumas coisas mudar�o de lugar em um futuro pr�ximo.
*Escrito por R�ger (rogerlasch@gmail.com) em julho de 2021.
*Este arquivo faz parte da dlb_utils, uma biblioteca de utilidades. Use por sua conta e risco!
**/

#ifndef DLB_CONSTANTS_H
#define DLB_CONSTANTS_H

#if !defined(DLB_TYPES_H)
#include"dlb_types.h"
#include"dlb_log.h"
#endif

namespace dlb
{

enum dlb_errors
{
dlb_error_ok=0,//Tudo certo
dlb_error_null, //o ponteiro passado � inv�lido ou � NULL
dlb_error_unknown,//Um erro misterioso!
dlb_error_func,//A fun��o passada � inv�lida. Normalmente callbacks
dlb_error_no_event,
dlb_error_reset, //Est� esperando que uma fun��o de limpeza seja chamada
dlb_error_handle, //O handle especificado � inv�lido
dlb_error_not_found,
dlb_error_illparam,//Um dos par�metros passados � inv�lido
dlb_error_open_file,//Erro ao abrir o arquivo
dlb_error_file_empty,//O arquivo informado para leitura est� vazio, est� corrompido, ou n�o possui o n�mero de bytes esperados.
dlb_error_invalid_state,//O estado atual � inv�lido para executar esta tarefa
dlb_error_incomplet_task //A tarefa ainda n�o terminou de ser executada
};

//Estados

enum dlb_active_states
{
dlb_state_hidle=0,//Est� pronto para ser usado novamente ou est� no estado inicial.
dlb_state_error,//Algum erro aconteceu
dlb_state_finished,//A tarefa foi conclu�da com sucesso
dlb_state_canceled,//A tarefa foi cancelada
dlb_state_waiting_for_cancelation, //A tarefa est� aguardando que o cancelamento seja efetivado antes de retornar.
dlb_state_verifying_params,//Verificando par�metros
dlb_state_verifying_file,//Est� abrindo um arquivo para leitura ou escrita.
dlb_state_reading_file,//Est� lendo dados de um arquivo.
dlb_stat_read_for_work, //Est� pronto para iniciar o trabalho
dlb_state_working //Est� trabalhando em algo.
};
}

#endif
