#include <bits/stdc++.h>
#define RF(filename) {freopen((filename), "r", stdin);}
#define WF(filename) {freopen((filename), "w", stdout);}
//@func translate .txt into .tmx in XML language.
int main(){
	RF("input.txt");
	WF("OutputMap.tmx");

	int width, height, W, H, layer;
	scanf("%d %d %d %d %d", &width, &height, &W, &H, &layer);
	puts("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	printf("<map version=\"1.0\" orientation=\"orthogonal\" width=\"%d\" height=\"%d\" tilewidth=\"100\" tileheight=\"40\">\n", width, height);
	puts(" <tileset firstgid=\"1\" source=\"Untitled.tsx\"/>");
	for (int lr = 1; lr <= layer; lr++){
		printf(" <layer name=\"Tile Layer %d\" width=\"%d\" height=\"%d\">\n", lr, width, height);
		puts("  <data>");
		int **mat; mat = new int*[height];
		for (int r = 0; r < height; r++){
			mat[r] = new int[width];
			for (int c = 0; c < width; c++) mat[r][c] = 0;
		}
		for (int r = 0; r < H; r++) for (int c = 0; c < W; c++)
			scanf("%d", &mat[height - lr - (H - 1 - r) * 2][c]);
		for (int r = 0; r < height; r++){
			for (int c = 0; c < width; c++)
				printf("   <tile gid=\"%d\"/>\n", mat[r][c]);
			delete mat[r];
		}
		delete mat;
		puts("  </data>");
		puts(" </layer>");
	}
	puts("</map>");
	return 0;
}
