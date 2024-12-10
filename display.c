/*
*  display.c:
* ȭ�鿡 ���� ������ ���
* ��, Ŀ��, �ý��� �޽���, ����â, �ڿ� ���� ���
* io.c�� �ִ� �Լ����� �����
*/

#include "display.h"
#include "io.h"

// ����� ������� �»��(topleft) ��ǥ
const POSITION resource_pos = { 0, 0 };
const POSITION map_pos = { 1, 0 };
const POSITION status_pos = { 1, 61 };   // ����â ���� ��ġ
const POSITION message_pos = { 19, 0 };  // �ý��� �޽���â ���� ��ġ
const POSITION command_pos = { 19, 61 }; // ���â ���� ��ġ


char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };


void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);
void display_status(OBJECT_SAMPLE obj);
void display_system_message(const char* message);
void display_commands(void);


void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_WIDTH][MAP_WIDTH],
	CURSOR cursor,
	OBJECT_SAMPLE obj,
	const char* message
) {
	display_resource(resource);
	display_map(map);
	display_cursor(cursor);
	display_status(obj);
	display_system_message(message);
	display_commands();
}

void display_status(OBJECT_SAMPLE obj) {
	gotoxy(status_pos);
	set_color(COLOR_DEFAULT);
	printf("���� ����:\n");
	printf("��ġ: (%d, %d)\n", obj.pos.row, obj.pos.column);
	printf("�ӵ�: %d\n", obj.speed);
	printf("������: (%d, %d)\n", obj.dest.row, obj.dest.column);
}

void display_system_message(const char* message) {
	gotoxy(message_pos);
	set_color(COLOR_DEFAULT);
	printf("�ý��� �޽���: %s\n", message);
}

void display_commands(void) {
	gotoxy(command_pos);
	set_color(COLOR_DEFAULT);
	printf("[1] �̵�  [2] ����  [3] ����  [q] ����\n");
}

void display_resource(RESOURCE resource) {
	set_color(COLOR_RESOURCE);
	gotoxy(resource_pos);
	printf("spice = %d/%d, population=%d/%d\n",
		resource.spice, resource.spice_max,
		resource.population, resource.population_max
	);
}

// subfunction of draw_map()
void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			for (int k = 0; k < N_LAYER; k++) {
				if (src[k][i][j] >= 0) {
					dest[i][j] = src[k][i][j];
				}
			}
		}
	}
}

void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	project(map, backbuf);

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (frontbuf[i][j] != backbuf[i][j]) {
				POSITION pos = {i, j };
				printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT);
			}
			frontbuf[i][j] = backbuf[i][j];
		}
	}
}

// frontbuf[][]���� Ŀ�� ��ġ�� ���ڸ� ���� �ٲ㼭 �״�� �ٽ� ���
void display_cursor(CURSOR cursor) {
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;

	char ch = frontbuf[prev.row][prev.column];
	printc(padd(map_pos, prev), ch, COLOR_DEFAULT);

	ch = frontbuf[curr.row][curr.column];
	printc(padd(map_pos, curr), ch, COLOR_CURSOR);
}