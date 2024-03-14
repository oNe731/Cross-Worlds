#include "stdafx.h"
#include "GameInstance.h"

#include "State_Enemy_VehicleFlying_Run.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"
#include "Vehicle_Flying.h"
#include "Vehicle_Flying_EnemyBiplane.h"
#include "Game_Manager.h"
#include "Player.h"

CState_Enemy_VehicleFlying_Run::CState_Enemy_VehicleFlying_Run(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_Enemy_VehicleFlying_Run::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pVehicle)
        return E_FAIL;

    return S_OK;
}

void CState_Enemy_VehicleFlying_Run::Enter_State(void* pArg)
{
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);

    CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
    if (nullptr == pPlayer)
        return;
    CCharacter* pCharacter = pPlayer->Get_Character();
    if (nullptr == pCharacter)
        return;

    m_pTarget = pCharacter;
    m_bUpdate = false;

    m_bEngineer = (wstring::npos != m_pVehicle->Get_ObjectTag().find(L"Vehicle_Flying_EnemyBiplane"));


    if (true == m_bEngineer)
    {
        CVehicle_Flying_EnemyBiplane* pBiplane = dynamic_cast<CVehicle_Flying_EnemyBiplane*>(m_pVehicle);

        if (nullptr != pBiplane)        
            pBiplane->Start_Trail();
    }
    
}

void CState_Enemy_VehicleFlying_Run::Tick_State(_float fTimeDelta)
{
    if (nullptr == m_pTarget)
        return;

    CVehicle_Flying* pFlying = dynamic_cast<CVehicle_Flying*>(m_pVehicle);
    if (nullptr == pFlying) // �����Ͼ� ������ϼ��� ����. Desc�� ������ �ͼ� �Ǵ��ؾ���. �ϴ� Boto
        return;

    if (true == pFlying->Get_Stat().bIsEnemy)
    {
        Vec4 vMyPos = m_pTransformCom->Get_Position();
        Vec4 vPlayerPos = m_pTarget->Get_CharacterTransformCom()->Get_Position();

        // 30 : ���� ������. 15 : ����. // 40 : Run (Temp)
        if (30.f > XMVectorGetX(XMVector3Length(vPlayerPos - vMyPos)))
        {
            m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_TRACE);
            return;
        }

        // �� ������ �ɸ��� ���� ��Ȳ������ �־��� Route���� ���ƴٴѴ�.
        
        Vec4 vDestPos = pFlying->Get_RoutePoint(pFlying->Get_CurIndex());
        Move(fTimeDelta); 

        _float fDistanceDest = (vDestPos - Vec4(m_pTransformCom->Get_Position())).Length();
        if (0.2f > fDistanceDest) // ���� ������ DestPosition�� ���� ����������
        {
            if (false == m_bUpdate)
            {
                _int iCurIndex = pFlying->Get_CurIndex();
                _int iMaxIndex = pFlying->Get_Routes()->size() - 1;
                _int iRandom = 0;

                // ������Ʈ ���̶��
                do {
                    iRandom = GI->RandomInt(0, iMaxIndex);

                } while (iCurIndex == iRandom);
                // ���� ���� ���ڰ� ������ �������´�.
                pFlying->Set_CurIndex(iRandom); // ���� ��θ� �����ϰ�

                m_bUpdate = true; // �Һ��� ����
            }
        }
        else
        {
            m_bUpdate = false;
        }

        if (true == m_bEngineer && (GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_MONSTER).size() <= 1))
        {
            m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_STAND);
            return;
        }
    }
}

void CState_Enemy_VehicleFlying_Run::Exit_State()
{
    m_pTarget = nullptr;
    m_bUpdate = false;

    if (true == m_bEngineer)
    {
        CVehicle_Flying_EnemyBiplane* pBiplane = dynamic_cast<CVehicle_Flying_EnemyBiplane*>(m_pVehicle);

        if (nullptr != pBiplane)
            pBiplane->Stop_Trail();
    }
}

void CState_Enemy_VehicleFlying_Run::Move(_float fTimeDelta)
{
    CVehicle_Flying* pBoto = dynamic_cast<CVehicle_Flying*>(m_pVehicle);
    if (nullptr == pBoto)
        return;

    // ���� �ε����� �޾Ƽ� ��ǥ �������� �����Ѵ�.
    _uint iIndex = pBoto->Get_CurIndex();
    Vec4 vDestPos = pBoto->Get_RoutePoint(iIndex);

    // ��Ʈ�� ¥�� ���� ������.
    Vec4 vMyPos = m_pTransformCom->Get_Position();

    if (0.1f < Vec3(vDestPos - vMyPos).Length())
    {
        Vec3 vDestLook = XMVector3Normalize(vDestPos - vMyPos);
        Vec3 vLook = XMVector3Normalize(m_pTransformCom->Get_Look());

        Vec3 vLerpLook = Vec3::Lerp(vLook, vDestLook, 5.f * fTimeDelta);

        m_pTransformCom->Rotation_Look(XMVector3Normalize(vLerpLook));
        m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fMovingSpeed, fTimeDelta);
    }
}

CState_Enemy_VehicleFlying_Run* CState_Enemy_VehicleFlying_Run::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Enemy_VehicleFlying_Run* pInstance = new CState_Enemy_VehicleFlying_Run(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Enemy_VehicleFlying_Run");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Enemy_VehicleFlying_Run::Free()
{
    __super::Free();
}
