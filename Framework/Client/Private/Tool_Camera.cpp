#include "stdafx.h"
#include "Tool_Camera.h"

#include "imgui.h"

#include "GameInstance.h"
#include "Camera_Manager.h"

#include "Camera.h"
#include "Camera_Free.h"
#include "Camera_Follow.h"

#include "Utils.h"
#include "FileUtils.h"

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
	Show_Select_Camera();
	


	/* Camera Prop */
	{
		CCamera* pCurCam = CCamera_Manager::GetInstance()->Get_CurCamera();
		if (nullptr != pCurCam)
		{
			/* ī�޶� ���� �ɼ� */
			Show_Camera_Prop_Default(pCurCam);

			/* ī�޶� ���� �ɼ� */
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

void CTool_Camera::Show_Select_Camera()
{
	const CAMERA_TYPE eCurCamIndex = (CAMERA_TYPE)CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Type();

	if (0 <= eCurCamIndex)
	{
		ImGui::Text("ī�޶� ����Ʈ");
		const char* Preview = CameraCharNames[eCurCamIndex];
		ImGui::PushItemWidth(150.f);
		if (ImGui::BeginCombo(u8"���� ī�޶�", Preview))
		{
			for (int iCurComboIndex = 0; iCurComboIndex < CAMERA_TYPE::CAMERA_TYPE_END; iCurComboIndex++)
			{
				const bool is_selected = (eCurCamIndex == iCurComboIndex);

				if (ImGui::Selectable(CameraCharNames[iCurComboIndex], is_selected))
				{
					CCamera_Manager::GetInstance()->Set_CurCamera((CAMERA_TYPE)iCurComboIndex);
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
	}
}

void CTool_Camera::Show_Camera_Prop_Default(CCamera* pCurCam)
{
	IMGUI_NEW_LINE;
	ImGui::Text("Camera Public Option (ī�޶� ���� �ɼ�)");

	if (ImGui::BeginChild("Public Camera Option", ImVec2(0, 450.f), true))
	{
		ImGui::PushItemWidth(150.f);
		{
			/* ���� ���� */
			{
				CCamera::PROJ_DESC tProjDesc = pCurCam->Get_ProjDesc();

				/* Fov */
				{
					_float fFov = XMConvertToDegrees(tProjDesc.tLerpFov.fCurValue);
					ImGui::DragFloat(u8"Fov", &fFov, 0.1f, 10.f, 100.f);
					tProjDesc.tLerpFov.fCurValue = XMConvertToRadians(fFov);

					/*if (ImGui::Button("Default Fov"))
						tProjDesc.tLerpFov.fCurValue = Cam_Fov_Follow_Default;*/
				}

				/* Aspect */
				{
					ImGui::DragFloat(u8"Aspect", &tProjDesc.fAspect, 0.05f, 0.1f, 10.f);

					if (ImGui::Button("Default Aspect"))
						tProjDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
				}

				/* Near */
				{
					ImGui::DragFloat(u8"Near", &tProjDesc.fNear, 0.05f, 0.01f, 1.f);

					/*if (ImGui::Button("Default Near"))
						tProjDesc.fNear = 0.2f;*/
				}

				/* Far */
				{
					ImGui::DragFloat(u8"Far", &tProjDesc.fFar, 0.05f, 1.f, 2000.f);

					/*if (ImGui::Button("Default Far"))
						tProjDesc.fFar = 1000.f;*/
				}

				pCurCam->Set_ProjDesc(tProjDesc);
			}
			IMGUI_NEW_LINE;

			/* ������ */
			{
				/* Ÿ�� ������ */
				{
					Vec4	vTargetOffset = pCurCam->Get_TargetOffset();
					_float	fTargetOffset[3] = { vTargetOffset.x, vTargetOffset.y, vTargetOffset.z };
					if (ImGui::DragFloat3(u8"Target OffSet", fTargetOffset))
					{
						pCurCam->Set_TargetOffSet(Vec4{ fTargetOffset[0], fTargetOffset[1], fTargetOffset[2], 1.f });
					}
				}

				/* ��� ������ */
				{
					Vec4	vLookAtOffset = pCurCam->Get_LookAtOffset();
					_float	fLookAtOffset[3] = { vLookAtOffset.x, vLookAtOffset.y, vLookAtOffset.z };
					if (ImGui::DragFloat3(u8"LookAt Offset", fLookAtOffset))
					{
						pCurCam->Set_LookAtOffSet(Vec4{ fLookAtOffset[0], fLookAtOffset[1], fLookAtOffset[2], 1.f });
					}
				}
			}
			IMGUI_NEW_LINE;

			/* �ΰ��� */
			{
				Vec2 vSeneitivity = pCurCam->Get_MouseSensitivity();

				ImGui::DragFloat(u8"Seneitivity X (X�� �ΰ���)", &vSeneitivity.x, 0.025f);
				ImGui::DragFloat(u8"Seneitivity Y (Y�� �ΰ���)", &vSeneitivity.y, 0.025f);

				pCurCam->Set_MouseSensitivity_X(vSeneitivity.x);
				pCurCam->Set_MouseSensitivity_Y(vSeneitivity.y);
			}
			IMGUI_NEW_LINE;

			/* ���Ͻ� */
			{
				_float fDistance = pCurCam->Get_Distance();
				if (ImGui::DragFloat(u8"Distance(Ÿ�ٰ��� �Ÿ�)", &fDistance, 0.5f, 0.1f, 100.f))
				{
					pCurCam->Set_Distance(fDistance);
				}
			}
			IMGUI_NEW_LINE;

			/* Shake */
			{
				_bool bShake = pCurCam->Is_Shake();
				ImGui::Checkbox(u8"Is Shake", &bShake);

				static _float fAmplitudeInput = 0.05f;
				ImGui::DragFloat(u8"Shake Amplitude (����ŷ ����) ", &fAmplitudeInput, 0.01f, 0.f, 5.f);

				static _float fFrquencyInput = 20.f;
				ImGui::DragFloat(u8"Shake Frquency (����ŷ ��) ", &fFrquencyInput, 0.01f, 0.f, 100.f);

				static _float fDurationInput = 0.3f;
				ImGui::DragFloat(u8"Shake Duration (����ŷ �ð�) ", &fDurationInput, 0.01f, 0.f, 10.f);

				if (ImGui::Button(u8"Start Shake(����ŷ ����)"))
				{
					pCurCam->Start_Shake(fAmplitudeInput, fFrquencyInput, fDurationInput);
				}
			}
		}
		ImGui::PopItemWidth();
		
	}
	ImGui::EndChild();
}

void CTool_Camera::Show_Camera_Prop_Free(CCamera* pCurCam)
{
	IMGUI_NEW_LINE;
	ImGui::Text("Free Camera Option (ī�޶� ���� �ɼ�)");

	if (ImGui::BeginChild("Free Camera Option", ImVec2(0, 200.f), true))
	{
		CCamera_Free* pFreeCam = dynamic_cast<CCamera_Free*>(pCurCam);
		if (nullptr != pFreeCam)
		{
			ImGui::PushItemWidth(150.f);
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
	ImGui::Text("Follow Camera Option (ī�޶� ���� �ɼ�)");

	if (ImGui::BeginChild("Follow Camera Option", ImVec2(0, 200.f), true))
	{
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(pCurCam);

		if (nullptr != pFollowCam)
		{
			ImGui::PushItemWidth(150.f);
			{
				/* ���� */
				{
					/* ��Ƽ�� */
					_bool bDamping = pFollowCam->Is_Damping();
					if (ImGui::Checkbox(u8"Damping On Off", &bDamping))
					{
						pFollowCam->Set_Damping(bDamping);
					}


					/* ��� */
					_float fCoefficient = pFollowCam->Get_DampingCoefficient();
					if (ImGui::DragFloat(u8"Damping Coefficient(���� ���)", &fCoefficient, 0.002f, 0.002f, 1.f))
					{
						pFollowCam->Set_DampingCoefficient(fCoefficient);
					}
				}
				IMGUI_NEW_LINE;

				/* ȸ�� */
				{
					/* Max Y */
					_float fMaxRotDeltaY = pFollowCam->Get_MaxRotationLimitDeltaY();
					if (ImGui::DragFloat(u8"Y�� ȸ�� ��Ÿ �ִ� ", &fMaxRotDeltaY, 0.01f, -1.f, 1.f))
					{
						pFollowCam->Set_MaxRotationLimitDeltaY(fMaxRotDeltaY);
					}

					/* Min Y */
					_float fMinRotDeltaY = pFollowCam->Get_MinRotationLimitDeltaY();
					if (ImGui::DragFloat(u8"Y�� ȸ�� ��Ÿ �ּڰ� ", &fMinRotDeltaY, 0.01f, -1.f, 1.f))
					{
						pFollowCam->Set_MinRotationLimitDeltaY(fMinRotDeltaY);
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
