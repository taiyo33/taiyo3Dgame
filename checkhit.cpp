//=============================================================================
//
// 衝突判定処理 [checkhit.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "main.h"
#include "checkhit.h"
#include "player.h"
#include "bullet.h"
#include "time.h"
#include "result.h"
#include "hitEffect.h"
#include "block.h" 
#include "explosion.h"
#include "field.h"
#include "child.h" 



D3DXVECTOR3				p3;
D3DXVECTOR3				nor;
//=============================================================================
// BBによる当たり判定処理
// 回転は考慮しない
// 戻り値：当たってたらtrue
//=============================================================================
void CheckHit(void)
{
	int i, j, k;	// ループカウント変数

	/* 対ブロックの当たり判定 */
	BLOCK *block = GetBlock(0);	// ブロックのアドレスを取得
	for (i = 0; i < BLOCK_MAX; i++, block++)
	{
		// ブロックとバレットの判定
		if (!block->use) continue;
		for (j = 0; j < BULLET_SET_MAX; j++)
		{
			CheckBlockHitBullet(i, j, block->pos);
		}
	}

	/* 対ブロックの当たり判定 */
	block = GetBlock(0);		// ブロックのアドレスを取得
	for (i = 0; i < BLOCK_NUM_FEARSIDE; i++, block++)
	{
		// P1の子供モデルとの判定
		CHILD *child = GetChild(P1);
		for (k = 0; k < CHILD_ONESET_MAX; k++)
		{
			if (!child->use[k]) continue;
			if (CheckHitBB(block->pos, child->pos[k], D3DXVECTOR3(15.0f,15.0f, 15.0f), D3DXVECTOR3(15.0f, 15.0f, 15.0f)))
			{
				child->pos[k] = child->prevPos[k];
			}
		}

		// P2の子供モデルとの判定
		child = GetChild(P2);
		for (k = 0; k < CHILD_ONESET_MAX; k++)
		{
			if (!child->use[k]) continue;
			if (CheckHitBB(block->pos, child->pos[k], D3DXVECTOR3(15.0f, 15.0f, 15.0f), D3DXVECTOR3(15.0f, 15.0f, 15.0f)))
			{
				child->pos[k] = child->prevPos[k];
			}
		}
	}


	/* 対プレイヤーの当たり判定 */
	PLAYER *player = GetPlayer(0);	// プレイヤーのアドレスを取得
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (!player->use) continue;

		// フィールドの４つ角との判定
		if (HitCheckCornerBlock(player->pos))
		{
			player->pos = player->prevPos;
		}
	}
	
	/* 対バレットの当たり判定 */
	BULLET *bullet = GetBullet(0);	//バレットのアドレスを取得
	player = GetPlayer(0);
	for (i = 0; i < BULLET_ONESET_MAX; i++)
	{
		// P1のバレット
		if (bullet[P1].use[i])
		{
			// P2の子供モデルとの判定
			CHILD *child = GetChild(0);
			for (j = 0; j < CHILD_ONESET_MAX; j++)
			{
				if (!child[P2].use[j]) continue;

				if (CheckHitBC(bullet[P1].pos[i], child[P2].pos[j], 15.0f, 15.0f))
				{
					PlaySound(child[P2].damageSE, E_DS8_FLAG_NONE);
					child[P2].use[j] = false;
					child[P2].cnt --;
					child[P1].cnt ++;
					SetChild(P1);
					SetExplosion(child[P2].pos[j], child[P2].rot[j], 0);
				}
			}
			
			// P2との判定
			if (CheckHitBC(bullet[P1].pos[i], player[P2].pos, 
								bullet[P1].size[i].x, 10.0f))
			{
				PlaySound(player[P2].hitSE, E_DS8_FLAG_NONE);
				SetHitEffect(bullet[P1].pos[i], D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
				player[P2].life -= BULLET_DAMAGE;
				bullet[P1].use[i] = false;
				bullet[P1].reflect[i] = false;
				bullet[P1].cntReflect[i] = INIT_REFLECT_CNT;
				bullet[P1].speed[i] = INIT_BULLET_SPEED;
			}
		}
		
		// P2のバレット
		if (bullet[P2].use[i])
		{
			// P1の子供モデルとの判定
			CHILD *child = GetChild(0);
			for (j = 0; j < CHILD_ONESET_MAX; j++)
			{
				if (!child[P1].use[j]) continue;

				if (CheckHitBC(bullet[P2].pos[i], child[P1].pos[j], 15.0f, 15.0f))
				{
					PlaySound(child[P1].damageSE, E_DS8_FLAG_NONE);
					child[P1].use[j] = false;
					child[P1].cnt --;
					child[P2].cnt ++;
					SetChild(P2);
					SetExplosion(child[P1].pos[j], child[P1].rot[j], 0);
				}
			}

			// P1との判定
			if (CheckHitBC(bullet[P2].pos[i], player[P1].pos,
								  bullet[P2].size[i].x, 10.0f))
			{
				PlaySound(player[P1].hitSE, E_DS8_FLAG_NONE);
				SetHitEffect(bullet[P2].pos[i], D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
				player[P1].life -= BULLET_DAMAGE;
				bullet[P2].use[i] = false;
				bullet[P2].reflect[i] = false;
				bullet[P2].cntReflect[i] = INIT_REFLECT_CNT;
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
// レイと球の当たり判定処理
// 引　数：
// 戻り値：当たってたらtrue
//=============================================================================
bool CheckHitRay(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 vec, float size)
{
	pos2 -= pos1;
	D3DXVECTOR3 vec1;

	D3DXVec3Normalize(&vec1, &vec);
	float dot1 = D3DXVec3Dot(&pos2, &vec1);
	float dot2 = D3DXVec3Dot(&pos2, &pos2);

	float s = dot1 * dot1 - dot2 + size * size;

	if (s < 0.0f)
	{
		return false;
	}

	float sq = sqrtf(s);
	float t = -dot1 - sq;

	if (t < 0.0f)
	{
		return false;
	}

	return true;
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

//===========================================================================
// 壁ずりのベクトル計算処理
// 引　数：D3DXVECTOR3 pos(次の移動位置)、D3DXVECTOR3 normal(ポリゴンの法線)
//		   int index(プレイヤーのアドレス番号)
// 戻り値：D3DXVECTOR3型 out
//==========================================================================
D3DXVECTOR3 WallShear(D3DXVECTOR3 pos, D3DXVECTOR3 normal, int index)
{
	PLAYER *player = GetPlayer(index);
	D3DXVECTOR3 normal_n;
	D3DXVECTOR3 frontVec, out;

	frontVec = pos - player->prevPos;
	D3DXVec3Normalize(&normal_n, &normal);
	D3DXVec3Normalize(&out, &(frontVec - D3DXVec3Dot(&frontVec, &normal_n) * normal_n));
	return out;
}

//==============================================================================
// 反射ベクトルの計算処理
// 引　数：D3DXVECTOR3 pos(位置)、D3DXVECTOR3 normal(ブロックの法線)、
// 戻り値：D3DXVECTOR3型
//==============================================================================
D3DXVECTOR3 ReflectVector(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 normal)
{
	D3DXVECTOR3 normal_n;
	D3DXVECTOR3 frontVec = pos0 - pos1;
	D3DXVECTOR3	out;

	D3DXVec3Normalize(&normal_n, &normal);
	D3DXVec3Normalize(&out, &(frontVec - (2.0f * D3DXVec3Dot(&frontVec, &normal_n)) * normal_n));
	return out;
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
//	float speed = speed0 - speed1;			// 衝突時の速度		
//	float dot;
//	D3DXVECTOR3 vec;
//	D3DXVec3Normalize(&c, &c);
//	dot = D3DXVec3Dot(&speed, &c);
//
//	vec = ((refRate * dot) / totalWeight) * c;
//	
//}

