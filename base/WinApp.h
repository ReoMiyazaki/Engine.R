#pragma once
#include <windows.h>

class WinApp
{
public: // �ÓI�����o�֐�
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	// �E�B���h�E�̃T�C�Y�ݒ�
	static const int window_width = 1280;	// ����
	static const int window_hright = 720;	// �c��

public: // �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �I��
	/// </summary>
	void Finalize();

	/// <summary>
	/// �E�B���h�E�n���h���̎擾
	/// </summary>
	/// <returns></returns>
	HWND GetHwnd() { return hwnd; }

	HINSTANCE GetInstance() { return w.hInstance; }

	/// <summary>
	/// ���b�Z�[�W�̏���
	/// </summary>
	/// <returns>�I�����ǂ���</returns>
	bool ProcessMessage();

private:
	HWND hwnd = nullptr;	// �E�B���h�E�n���h��
	WNDCLASSEX w{};	// �E�B���h�E�N���X�̐ݒ�
};