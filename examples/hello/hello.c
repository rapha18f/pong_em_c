/* Exemplo: Desenhar texto na tela usando KOS */
#include <kos.h>
#include <stdio.h>

/* A KOS precisa de uma textura de fonte para desenhar texto.
   A função pvr_font_draw() usa a fonte padrão. */

extern uint8 romdisk[];  // Romdisk com os assets
KOS_INIT_ROMDISK(romdisk);  // Inicializa o sistema de arquivos

int main(int argc, char **argv) {
    printf("Iniciando PONG com texto na tela...\n");
    
    /* Inicializa o PVR (PowerVR) - o sistema gráfico do Dreamcast */
    pvr_init_defaults();
    
    /* Loop principal do jogo */
    while (1) {
        /* Verifica se o usuário quer sair (apertou Start) */
        maple_device_t *cont;
        cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
        if (cont) {
            cont_state_t *state = (cont_state_t *)maple_dev_status(cont);
            if (state && (state->buttons & CONT_START)) {
                break;  // Sai do loop
            }
        }
        
        /* =============================================
           INÍCIO DA RENDERIZAÇÃO
           ============================================= */
        
        /* Aguarda o próximo frame */
        pvr_wait_ready();
        pvr_scene_begin();
        
        /* === LIMPA A TELA === */
        pvr_list_begin(PVR_LIST_OP_POLY);
        pvr_prim_t prim;
        pvr_poly_cxt_t cxt;
        
        /* Fundo preto */
        pvr_poly_cxt_col(&cxt, PVR_LIST_OP_POLY, 0xFF000000);
        pvr_poly_prim(&prim, &cxt);
        pvr_vert_t vert[4] = {
            {0, 0, 0, 1, 0xFFFFFFFF, 0, 0},
            {640, 0, 0, 1, 0xFFFFFFFF, 0, 0},
            {0, 480, 0, 1, 0xFFFFFFFF, 0, 0},
            {640, 480, 0, 1, 0xFFFFFFFF, 0, 0}
        };
        pvr_prim_list(&prim, vert, 4);
        pvr_list_finish();
        
        /* === DESENHA O TEXTO NA TELA === */
        /* A função pvr_font_draw() desenha texto usando a fonte padrão.
           Parâmetros: (x, y, cor_ABGR, "texto", alinhamento) */
        
        pvr_font_draw(10, 10, 0xFFFFFFFF, "Hello World!");
        pvr_font_draw(10, 40, 0xFFFFFF00, "PONG em C");
        pvr_font_draw(10, 70, 0xFF00FF00, "Aperte START para sair");
        
        /* === FINALIZA A RENDERIZAÇÃO === */
        pvr_scene_finish();
    }
    
    /* Desliga o PVR */
    pvr_shutdown();
    
    printf("Fim do jogo!\n");
    return 0;
}