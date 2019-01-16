#include "main.h"
#include "checkhit.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "score.h"
#include "result.h"
#include "effect.h"
#include "block.h" 
#include "explosion.h"
#include "field.h"

D3DXVECTOR3				p3;
D3DXVECTOR3				nor;
//=============================================================================
// BBによる当たり判定処理
// 回転は考慮しない
// 戻り値：当たってたらtrue
//=============================================================================
void CheckHit(void)
{
	int i,j;

	/* ブロック対バレットの当たり判定 */
	BLOCK *block = GetBlock(0);	// ブロックのアドレスを取得
	for (j = 0; j < BLOCK_MAX; j++, block++)
	{
		if (!block->use) continue;

		BULLET *bullet = GetBullet(0);	//バレットのアドレスを取得
		for (i = 0; i < BULLET_SET_MAX; i++)
		{
			// プレイヤー１のバレット
			if (!bullet[P1].use[i]) continue;
			if (CheckHitBB(bullet[P1].pos[i], block->pos,
				D3DXVECTOR3(3.0f, 3.0f, 3.0f), D3DXVECTOR3(25.0f, 25.0f, 25.0f)))
			{
				BlockDamageManager(j);
				bullet[P1].use[i] = false;
			}
			// プレイヤー２のバレット
			if (!bullet[P2].use[i]) continue;
			if (CheckHitBB(bullet[P2].pos[i], block->pos,
				D3DXVECTOR3(3.0f, 3.0f, 3.0f), D3DXVECTOR3(25.0f, 25.0f, 25.0f)))
			{
				BlockDamageManager(j);
				bullet[P2].use[i] = false;
			}
		}
	}

	/* 対プレイヤーの当たり判定 */
	PLAYER *player = GetPlayer(0);
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (!player->use) continue;
		
		// プレイヤーのフィール内外判定
		if (!CheckFieldInPlayer(i))
		{
			player->pos.y -= 5.0f;
			PlayerDamageManager(i);
		}

		// ブロックとの当たり判定
		BLOCK *block = GetBlock(0);
		for (j = 0; j < BLOCK_MAX; j++, block++)
		{
			if (!block->use) continue;
			
			if (CheckHitBB(player->pos, block->pos,
				D3DXVECTOR3(25.0f, 25.0f, 25.0f), D3DXVECTOR3(25.0f, 100.0f, 25.0f)))
			{
				player->pos = player->prevPos;
			}
		}

		BULLET *bullet = GetBullet(0);	//バレットのアドレスを取得
		for (j = 0; j < BULLET_SET_MAX; j++)
		{
			// プレイヤー１のバレット
			if (!bullet[P1].use[i]) continue;
			if (CheckHitBB(bullet[P1].pos[j], player[P2].pos,
				D3DXVECTOR3(3.0f, 3.0f, 3.0f), D3DXVECTOR3(25.0f, 25.0f, 25.0f)))
			{
				PlayerDamageManager(j);
				bullet[P1].use[i] = false;
			}
			// プレイヤー２のバレット
			if (!bullet[P2].use[i]) continue;
			if (CheckHitBB(bullet[P2].pos[j],player[P1].pos,
				D3DXVECTOR3(3.0f, 3.0f, 3.0f), D3DXVECTOR3(25.0f, 25.0f, 25.0f)))
			{
				PlayerDamageManager(j);
				bullet[P2].use[i] = false;
			}
		}
	}
}

//=============================================================================
// BBによる当たり判定処理
// 回転は考慮しない
// 戻り値：当たってたらtrue
//=============================================================================
bool CheckHitBB(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 size1, D3DXVECTOR3 size2)
{
	size1 /= 2.0f;	// 半サイズにする
	size2 /= 2.0f;	// 同上

	if (pos2.x + size2.x > pos1.x - size1.x && pos1.x + size1.x > pos2.x - size2.x &&
		pos2.y + size2.y > pos1.y - size1.y && pos1.y + size1.y > pos2.y - size2.y &&
		pos2.z + size2.z > pos1.z - size1.z && pos1.z + size1.z > pos2.z - size2.z)
	{
		return true;
	}

	return false;
}

//=============================================================================
// BCによる当たり判定処理
// サイズは半径
// 戻り値：当たってたらtrue
//=============================================================================
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2)
{
	if ((size1 + size2) * (size1 + size2) >
		(pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y) + (pos1.z - pos2.z) * (pos1.z - pos2.z))
	{
		return true;
	}

	return false;
}

//=============================================================================
// 線分とポリゴンの当たり判定処理
// 引　数： D3DXVECTOR3 p0,p1,p2(ポリゴンの各頂点), D3DXVECTOR3 pos0(始点)
//			D3DXVECTOR3 pos1(終点)
// 戻り値： bool型 当たっていればfalse, そうでなければtrue
//=============================================================================
bool CheckHitPolygon(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	
	// ポリゴンの外積
	D3DXVECTOR3		vec01 = p1 - p0;
	D3DXVECTOR3		vec02 = p2 - p0;
	D3DXVec3Cross(&nor, &vec01, &vec02);
	D3DXVec3Normalize(&nor, &nor);
	
	// 平面と線分の内積
 	D3DXVECTOR3		vec1 = pos0 - p0;
	D3DXVECTOR3		vec2 = pos1 - p0;
	float dot1 = D3DXVec3Dot(&vec1, &nor);
	float dot2 = D3DXVec3Dot(&vec2, &nor);

	// ポリゴンの表裏判定
	if ((dot1 * dot2) > 0)
	{
		return true;
	}

	// ポリゴンと線分の交点を求める
	float d1 = fabsf(D3DXVec3Dot(&nor, &vec1));		// 点とポリゴンとの距離
	float d2 = fabsf(D3DXVec3Dot(&nor, &vec2));		// 点とポリゴンとの距離
	float a = d1 / (d1 + d2);						// 内分比
	D3DXVECTOR3	vec3 = (1 - a) * vec1 + a * vec2;	// 交点へのベクトル
	p3 = p0 + vec3;									// 交点

	// ポリゴンの各辺のベクトル
	D3DXVECTOR3		v1 = p1 - p0;
	D3DXVECTOR3		v2 = p2 - p1;
	D3DXVECTOR3		v3 = p0 - p2;

	// 各頂点と交点とのベクトル
	D3DXVECTOR3		v4 = p3 - p1;
	D3DXVECTOR3		v5 = p3 - p2;
	D3DXVECTOR3		v6 = p3 - p0;
	
	// 各辺との外積で法線を求める
	D3DXVECTOR3		n1, n2, n3;
	D3DXVec3Cross(&n1, &v1, &v4);
	D3DXVec3Cross(&n2, &v2, &v5);
	D3DXVec3Cross(&n3, &v3, &v6);

	// それぞれのベクトルの向き
	float dot12, dot23, dot31;

	dot12 = D3DXVec3Dot(&n1, &nor);
	dot23 = D3DXVec3Dot(&n2, &nor);
	dot31 = D3DXVec3Dot(&n3, &nor);

	// 交点の内外判定
	if (dot12 < 0) return true;
	if (dot23 < 0) return true;
	if (dot31 < 0) return true;

 	return false;
}

//========================================================================
// 交点の取得
// 引　数：なし
// 戻り値：D3DXVECTOR3型
//========================================================================
D3DXVECTOR3 GetP3(void)
{
	return p3;
}

//========================================================================
// 法線の取得
// 引　数：なし
// 戻り値：D3DXVECTOR3型
//========================================================================
D3DXVECTOR3 GetNormal(void)
{
	return nor;
}

//========================================================================
// オブジェクトの衝突後位置計算処理
//
//
//========================================================================
//void Reperlling(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, float weight0, float weight1, float speed0, float speed1)
//{
//	float totalWeight = weight0 + weight1;	// 総質量
//	float refRate = 1.10f;					// 反発率
//	D3DXVECTOR3 c = pos1 - pos0;			// 衝突軸ベクトル
//	float dot;
//	D3DXVECTOR3 vec;
//	D3DXVec3Normalize(&c, &c);
//	dot = D3DXVec3Dot(&(speed0 - speed1), &c);
//
//	vec = ((refRate * dot) / totalWeight) * c;
//	
//}


////========================================================================
//// 壁ずり
////
////
////========================================================================
//
//D3DXVECTOR3 WallShear(D3DXVECTOR3* out, const D3DXVECTOR3& front, const D3DXVECTOR3& normal) {
//	D3DXVECTOR3 normal_n;
//	D3DXVec3Normalize(&normal_n, &normal);
//	return D3DXVec3Normalize(out, &(front - D3DXVec3Dot(&front, &normal_n) * normal_n));
//}
//
////////////////////////////////////
//// 反射ベクトル
////
//// out : 正規化反射ベクトル（戻り値）
//// front : 進行ベクトル
//// normal: 衝突点での法線ベクトル
////
//D3DXVECTOR3* calcReflectVector(D3DXVECTOR3* out, const D3DXVECTOR3& front, const D3DXVECTOR3& normal) {
//	D3DXVECTOR3 normal_n;
//	D3DXVec3Normalize(&normal_n, &normal);
//	return D3DXVec3Normalize(out, &(front - 2.0f * D3DXVec3Dot(&front, &normal_n) * normal_n));
//}


//=============================================================================
// 当たり判定
// 引　数： D3DXVECTOR3 pos0(始点), D3DXVECTOR3 pos1(終点), float sizeX(横幅)
//			float sizeY(縦幅)
// 戻り値： bool型　当たっていればfalse、そうでなければtrue
//=============================================================================
//bool Check(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, float sizeX, float sizeY)
//{
//	D3DXVECTOR3 v1, v2, line;
//	float dot1, dot2;
//
//	// 線分の始点と終点
//	v1 = pos0; 
//	v2 = pos1; 
//
//	// 始点と終点の内積
//	dot1 = D3DXVec3Dot(&nor, &v1);
//	dot2 = D3DXVec3Dot(&nor, &v2);
//
//	line = v2 - v1;						// 線分の長さ
//	float a = fabs(dot1) / fabs(dot1) + fabs(dot2); // 内分比
//	D3DXVECTOR3 P = v1 + (a * line);	// 交点
//
//	// 表裏判定
//	if (dot1 * dot2 > 0)
//	{
//		return false;
//	}
//
//	D3DXVECTOR3 vec1 = D3DXVECTOR3(-sizeX, 0.0f, sizeY);
//	D3DXVECTOR3 vec2 = D3DXVECTOR3(-sizeX, 0.0f, -sizeY);
//	D3DXVECTOR3 vec3 = D3DXVECTOR3(sizeX, 0.0f, -sizeY);
//	D3DXVECTOR3 vec4 = D3DXVECTOR3(sizeX, 0.0f, sizeY);
//	D3DXVECTOR3 no1, no2, no3, no4;
//
//	float dot12, dot23, dot34;
//
//	D3DXVec3Cross(&no1, &(vec2 - vec1), &(P - vec2));
//	D3DXVec3Cross(&no2, &(vec3 - vec2), &(P - vec3));
//	D3DXVec3Cross(&no3, &(vec4 - vec3), &(P - vec4));
//	D3DXVec3Cross(&no4, &(vec1 - vec4), &(P - vec1));
//
//	dot12 = D3DXVec3Dot(&no1, &no2);
//	dot23 = D3DXVec3Dot(&no2, &no3);
//	dot34 = D3DXVec3Dot(&no3, &no4);
//
//	// 交点の内外判定
//	if (dot12 > 0 && dot23 > 0 && dot34 > 0)
//	{
//		return true;
//	}
//
//	return false;
//}

