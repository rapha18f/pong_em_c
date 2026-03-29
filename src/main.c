#include <kos.h>
#include <dc/pvr.h>
#include <dc/maple.h>
#include <dc/maple/controller.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    float x, y;
    float vx, vy;
} Ball;

pvr_poly_hdr_t hdr;

void setup_pvr() {
    pvr_poly_cxt_t cxt;
    pvr_poly_cxt_col(&cxt, PVR_LIST_OP_POLY);
    pvr_poly_compile(&hdr, &cxt);
}

void draw_rect(float x, float y, float w, float h, uint32_t color) {
    pvr_vertex_t v;
    v.z = 1.0f;
    v.argb = color;
    v.oargb = 0;
    v.flags = PVR_CMD_VERTEX;

    v.x = x; v.y = y;
    pvr_prim(&v, sizeof(v));
    v.x = x + w; v.y = y;
    pvr_prim(&v, sizeof(v));
    v.x = x; v.y = y + h;
    pvr_prim(&v, sizeof(v));
    v.flags = PVR_CMD_VERTEX_EOL;
    v.x = x + w; v.y = y + h;
    pvr_prim(&v, sizeof(v));
}

void reset_ball(Ball *b) {
    b->x = 320.0f;
    b->y = 240.0f;
    b->vx = (rand() % 2 ? 4.0f : -4.0f);
    b->vy = (rand() % 2 ? 3.0f : -3.0f);
}

int main(int argc, char **argv) {
    pvr_init_defaults();
    setup_pvr();

    Ball ball;
    reset_ball(&ball);

    float p1_y = 200.0f, p2_y = 200.0f;
    int score1 = 0, score2 = 0;
    char score_str[32];

    while (1) {
        // --- INPUT ---
        maple_device_t *cont1 = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
        maple_device_t *cont2 = maple_enum_type(1, MAPLE_FUNC_CONTROLLER);
        
        cont_state_t *st1 = (cont1) ? (cont_state_t *)maple_dev_status(cont1) : NULL;
        cont_state_t *st2 = (cont2) ? (cont_state_t *)maple_dev_status(cont2) : NULL;

        // Player 1 (Esquerda)
        if (st1) {
            if (st1->buttons & CONT_DPAD_UP) p1_y -= 5.0f;
            if (st1->buttons & CONT_DPAD_DOWN) p1_y += 5.0f;
            if (st1->buttons & CONT_START) break; 
        }

        // Player 2 ou IA (Direita)
        if (st2) {
            // Se o controle 2 estiver conectado, o humano assume
            if (st2->buttons & CONT_DPAD_UP) p2_y -= 5.0f;
            if (st2->buttons & CONT_DPAD_DOWN) p2_y += 5.0f;
        } else {
            // Se não houver controle 2, a IA joga
            if (ball.y < p2_y + 35) p2_y -= 3.2f;
            else if (ball.y > p2_y + 45) p2_y += 3.2f;
        }

        // Limites
        if (p1_y < 0) p1_y = 0; if (p1_y > 400) p1_y = 400;
        if (p2_y < 0) p2_y = 0; if (p2_y > 400) p2_y = 400;

        // --- LÓGICA DA BOLA ---
        ball.x += ball.vx;
        ball.y += ball.vy;

        if (ball.y <= 0 || ball.y >= 470) ball.vy = -ball.vy;

        // Colisões simplificadas para evitar "atravessar" o paddle
        if (ball.x < 70 && ball.x > 50 && ball.y + 10 > p1_y && ball.y < p1_y + 80) {
            ball.vx = -ball.vx;
            ball.x = 71; // Garante que a bola saia de dentro do paddle
            ball.vx *= 1.05f;
        }
        if (ball.x > 560 && ball.x < 580 && ball.y + 10 > p2_y && ball.y < p2_y + 80) {
            ball.vx = -ball.vx;
            ball.x = 559;
            ball.vx *= 1.05f;
        }

        if (ball.x < 0) { score2++; reset_ball(&ball); }
        if (ball.x > 640) { score1++; reset_ball(&ball); }

        // --- RENDERIZAÇÃO ---
        pvr_wait_ready();
        pvr_scene_begin();
        pvr_list_begin(PVR_LIST_OP_POLY);
        
        pvr_prim(&hdr, sizeof(hdr));
        draw_rect(50, p1_y, 20, 80, 0xFF00FF00);    // Verde
        draw_rect(570, p2_y, 20, 80, 0xFFFF0000);   // Vermelho
        draw_rect(ball.x, ball.y, 10, 10, 0xFFFFFFFF);

        pvr_list_finish();
        pvr_scene_finish();

        // Escrever o placar DEPOIS do finish para garantir que apareça sobre o PVR
        sprintf(score_str, "%02d  -  %02d", score1, score2);
        // Centralizado no topo (320 pixels é o meio, bfont tem largura variável mas 80 de offset ajuda)
        bfont_draw_str(vram_s + (20 * 640) + 260, 640, 0, score_str);
    }

    return 0;
}