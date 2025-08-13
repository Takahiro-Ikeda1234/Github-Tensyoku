using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Marker : MonoBehaviour
{
	private float m_Speed = 4f;
	private Vector3 m_PrevPosition;
	private SpriteRenderer m_Renderer;

	void Start()
	{
		// 移動前の初期位置を保存
		m_PrevPosition = transform.position;
		m_Renderer = GetComponent<SpriteRenderer>();

	}

	// 移動
	public void Move()
	{
		// プレイヤーの座標を取得
		Vector3 position = transform.position;

		// 移動前の位置を保存する
		m_PrevPosition = position;

		// 移動量を加える
		// 斜め移動を制限するため、いずれかの方向キーの入力のみ処理する
		if (Input.GetKey(KeyCode.W))
		{
			position.y += m_Speed;
		}
		else if (Input.GetKey(KeyCode.S))
		{
			position.y -= m_Speed;
		}
		else if (Input.GetKey(KeyCode.A))
		{
			position.x -= m_Speed;
		}
		else if (Input.GetKey(KeyCode.D))
		{
			position.x += m_Speed;
		}

		transform.position = position;
	}

	// 一つ前の座標を返す
	public Vector3 GetPrevousPosition()
	{
		return m_PrevPosition;
	}

	public void TakeDamage()
	{
		// 点滅によるダメージ演出
		StartCoroutine(Blink());
	}

	private IEnumerator Blink()
	{
		// 変更前の色のコピーを保存
		var original = m_Renderer.color;

		// 白色に光らせる
		m_Renderer.color = new Color(1, 0, 0);

		// 0.1秒待機して元の色に戻す
		yield return new WaitForSeconds(0.5f);
		m_Renderer.color = original;
	}
}
