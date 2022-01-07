

/**
*Algumas constantes usado pela dlb. Este arquivo ainda é versão beta e algumas coisas mudarão de lugar em um futuro próximo.
*Escrito por Róger (rogerlasch@gmail.com) em julho de 2021.
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
dlb_error_null, //o ponteiro passado é inválido ou é NULL
dlb_error_unknown,//Um erro misterioso!
dlb_error_func,//A função passada é inválida. Normalmente callbacks
dlb_error_no_event,
dlb_error_reset, //Está esperando que uma função de limpeza seja chamada
dlb_error_handle, //O handle especificado é inválido
dlb_error_not_found,
dlb_error_illparam,//Um dos parâmetros passados é inválido
dlb_error_open_file,//Erro ao abrir o arquivo
dlb_error_file_empty,//O arquivo informado para leitura está vazio, está corrompido, ou não possui o número de bytes esperados.
dlb_error_invalid_state,//O estado atual é inválido para executar esta tarefa
dlb_error_incomplet_task //A tarefa ainda não terminou de ser executada
};

//Estados

enum dlb_active_states
{
dlb_state_hidle=0,//Está pronto para ser usado novamente ou está no estado inicial.
dlb_state_error,//Algum erro aconteceu
dlb_state_finished,//A tarefa foi concluída com sucesso
dlb_state_canceled,//A tarefa foi cancelada
dlb_state_waiting_for_cancelation, //A tarefa está aguardando que o cancelamento seja efetivado antes de retornar.
dlb_state_verifying_params,//Verificando parâmetros
dlb_state_verifying_file,//Está abrindo um arquivo para leitura ou escrita.
dlb_state_reading_file,//Está lendo dados de um arquivo.
dlb_stat_read_for_work, //Está pronto para iniciar o trabalho
dlb_state_working //Está trabalhando em algo.
};
}

#endif
