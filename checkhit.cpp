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
	int i, j;

	/* ブロック対バレットの当たり判定 */
	BLOCK *block = GetBlock(0);	// ブロックのアドレスを取得
	for (i = 0; i < BLOCK_MAX; i++, block++)
	{
		if (!block->use) continue;
		for (j = 0; j < BULLET_SET_MAX; j++)
		{
			CheckBlockHitBullet(i, j, block->pos);
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
			PlayerDamageManager(i);
			player->pos.y -= 5.0f;
		}

		if (HitCheckCornerBlock(player->pos))
		{
			player->pos = player->prevPos;
		}

	}
	
	BULLET *bullet = GetBullet(0);	//バレットのアドレスを取得
	player = GetPlayer(0);
	for (i = 0; i < BULLET_ONESET_MAX; i++)
	{
		// プレイヤー１のバレット
		if (bullet[P1].use[i])
		{
			if (CheckHitBC(bullet[P1].pos[i], player[P2].pos,
				bullet[P1].size[j].x, 25.0f))
			{
				player[P2].life += 5.0f;
				bullet[P1].use[i] = false;
				bullet[P1].reflect[i] = false;
				bullet[P1].cntReflect[i] = 2;
			}
		}
		// プレイヤー２のバレット
		if (bullet[P2].use[i])
		{
			if (CheckHitBC(bullet[P2].pos[i], player[P1].pos,
				bullet[P2].size[j].x, 25.0f))
			{
				player[P1].life -= 5.0f;
				bullet[P2].use[i] = false;
				bullet[P2].reflect[i] = false;
				bullet[P2].cntReflect[i] = 2;
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



