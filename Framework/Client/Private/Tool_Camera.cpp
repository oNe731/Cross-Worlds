#include "stdafx.h"
#include "Tool_Camera.h"

#include "imgui.h"

#include "GameInstance.h"
#include "Camera_Manager.h"


#include "Camera.h"
#include "Camera_Free.h"
#include "Camera_Follow.h"

#include "Utils.h"

CTool_Camera::CTool_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTool(pDevice, pContext)
{
}

HRESULT CTool_Camera::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;


	return S_OK;
}

void CTool_Camera::Tick(_float fTimeDelta)
{
	ImGui::Begin("Camera_Tool");

	/* Select Camera */
	{
		const CAMERA_TYPE eCurCamIndex = (CAMERA_TYPE)CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Type();

		if (0 <= eCurCamIndex)
		{
			const char* Preview = CameraCharNames[eCurCamIndex];
			ImGui::PushItemWidth(150.f);
			if (ImGui::BeginCombo(u8"���� ���õ� ī�޶�", Preview))
			{
				for (int iCurComboIndex = 0; iCurComboIndex < CAMERA_TYPE::TYPE_END; iCurComboIndex++)
				{
					const bool is_selected = (eCurCamIndex == iCurComboIndex);

					if (ImGui::Selectable(CameraCharNames[iCurComboIndex], is_selected))
						CCamera_Manager::GetInstance()->Set_CurCamera((CAMERA_TYPE)iCurComboIndex);

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
		}
	}

	/* Show Camera Prop */
	{
		CCamera* pCurCam = CCamera_Manager::GetInstance()->Get_CurCamera();
		if (nullptr != pCurCam)
		{
			switch (pCurCam->Get_Type())
			{
			case CAMERA_TYPE::FREE :
				Show_Camera_Prop_Free(pCurCam);
				break;
			case CAMERA_TYPE::FOLLOW :
				Show_Camera_Prop_Follow(pCurCam);
				break;
			default:
				break;
			}
		}
	}

	ImGui::End();
}

void CTool_Camera::Show_Camera_Prop_Free(CCamera* pCurCam)
{
	IMGUI_NEW_LINE;
	ImGui::Text("ī�޶� �ɼ� ����");

	if (ImGui::BeginChild("Free Camera Option", ImVec2(0, 300.f), true))
	{
		CCamera_Free* pFreeCam = dynamic_cast<CCamera_Free*>(pCurCam);
		if (nullptr != pFreeCam)
		{
			ImGui::PushItemWidth(100.f);
			{
				/* ī�޶� �̵��ӵ� */
				_float fCameraMoveSpeed = pFreeCam->Get_MoveSpeed();

				if (ImGui::DragFloat(u8"ī�޶� �̵� �ӵ�", &fCameraMoveSpeed, 0.5f))
				{
					pFreeCam->Set_MoveSpeed(fCameraMoveSpeed);
				}

				/* ī�޶� ȸ�� �ӵ� */
				_float fCameraRotateSpeed = pFreeCam->Get_RotateSpeed();
				if (ImGui::DragFloat(u8"ī�޶� ȸ�� �ӵ�", &fCameraRotateSpeed, 0.1f))
				{
					pFreeCam->Set_RotateSpeed(fCameraRotateSpeed);
				}
			}
			ImGui::PopItemWidth();
		}
	}
	ImGui::EndChild();
}

void CTool_Camera::Show_Camera_Prop_Follow(CCamera* pCurCam)
{
	IMGUI_NEW_LINE;
	ImGui::Text("ī�޶� �ɼ� ����");

	if (ImGui::BeginChild("Follow Camera Option", ImVec2(0, 700.f), true))
	{
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(pCurCam);

		if (nullptr != pFollowCam)
		{
			ImGui::PushItemWidth(150.f);
			{
				
				/* ���� ���� */
				{
					CCamera::PROJ_DESC tProjDesc = pFollowCam->Get_ProjDesc();

					/* Fov */
					{
						ImGui::DragFloat(u8"Fov", &tProjDesc.tLerpFov.fCurValue, 0.05f, 0.1, 10.f);
					
						/*if (ImGui::Button("Default Fov"))
							tProjDesc.tLerpFov.fCurValue = Cam_Fov_Follow_Default;*/
					}

					/* Aspect */
					{
						ImGui::DragFloat(u8"Aspect", &tProjDesc.fAspect, 0.05f, 0.f, 10.f);

						/*if (ImGui::Button("Default Aspect"))
							tProjDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;*/
					}

					/* Near */
					{
						ImGui::DragFloat(u8"Near", &tProjDesc.fNear, 0.05f, 0.01f, 1.f);

						/*if (ImGui::Button("Default Near"))
							tProjDesc.fNear = 0.2f;*/
					}

					/* Far */
					{
						ImGui::DragFloat(u8"Far", &tProjDesc.fFar, 0.05f, 0.f, 2000.f);

						/*if (ImGui::Button("Default Far"))
							tProjDesc.fFar = 1000.f;*/
					}

					pFollowCam->Set_ProjDesc(tProjDesc);
				}

				/* ������ */
				{
					/* Ÿ�� ������ */
					{
						Vec4	vTargetOffset		= pFollowCam->Get_TargetOffset();
						_float	fTargetOffset[3]	= { vTargetOffset.x, vTargetOffset.y, vTargetOffset.z };
						if (ImGui::DragFloat3(u8"Ÿ�� ������(������)", fTargetOffset))
						{
							pFollowCam->Set_TargetOffSet(Vec4{ fTargetOffset[0], fTargetOffset[1], fTargetOffset[2], 1.f });
						}
					}
					
					/* ��� ������ */
					{
						Vec4	vLookAtOffset		= pFollowCam->Get_LookAtOffset();
						_float	fLookAtOffset[3]	= { vLookAtOffset.x, vLookAtOffset.y, vLookAtOffset.z };
						if (ImGui::DragFloat3(u8"��� ������(������)", fLookAtOffset))
						{
							pFollowCam->Set_LookAtOffSet(Vec4{ fLookAtOffset[0], fLookAtOffset[1], fLookAtOffset[2], 1.f });
						}
					}
				}

				/* �ΰ��� */
				{
					Vec2 vSeneitivity = pFollowCam->Get_MouseSensitivity();

					ImGui::DragFloat(u8"X�� �ΰ���", &vSeneitivity.x, 0.025f);
					ImGui::DragFloat(u8"Y�� �ΰ���", &vSeneitivity.y, 0.025f);

					pFollowCam->Set_MouseSensitivity_X(vSeneitivity.x);
					pFollowCam->Set_MouseSensitivity_Y(vSeneitivity.y);

				}


				/* ���Ͻ� */
				{
					_float fDistance = pFollowCam->Get_Distance();
					if (ImGui::DragFloat(u8"Distance(Ÿ�ٰ��� �Ÿ�)", &fDistance, 0.5f, 0.1f, 100.f))
					{
						pFollowCam->Set_Distance(fDistance);
					}
				}



				














			}
			ImGui::PopItemWidth();
		}
	}
	ImGui::EndChild();
}

CTool_Camera* CTool_Camera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Camera* pInstance = new CTool_Camera(pDevice, pContext);
	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed_Create : CTool_Camera");
		return nullptr;
	}
	return pInstance;
}

void CTool_Camera::Free()
{
	__super::Free();
}
