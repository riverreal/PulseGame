#include "RhythmManager.h"
#include "../Source/System/GameManager.h"
#include "../Source/Includes/LESystem.h"

using namespace Elixir;

/*
	�^�C�~���O����hitbox�ɉ����G�t�F�N�g���ق����H
	�f�ނ̂������킩��Ȃ����ǃ^�C�~���O��������Â炢�H
	�C�ӂ̃^�C�~���O�ōĐ�������
	comboUI�Ȃǂق����B
*/
void RhythmManager::Initialize(Elixir::SceneManager * sceneManager)
{
	Manager = sceneManager;
	//BGM�t�@�C���ݒ�
	AudioManager::GetInstance().AddControlledMusic("Resource/rhythmFolder/Dash.mp3");
	AudioManager::GetInstance().GetControlledMusic()->setIsPaused(true);

	//PNF�t�@�C���Q�b�g
	auto FileContents = Manager->GetFileManager()->LoadFileLines("Resource/rhythmFolder/Pnf_Folder/"+ FILE_NAME +".pnf");
	//line��,�ŕ������đ��
	for (auto & line : FileContents)
	{
		auto val = split(line, ',');
		m_NotesLaneNumber.push_back(std::stoi(val[0]));
		m_NotesTiming.push_back(std::stold(val[1]));
	}
	
	//�摜���擾���ă��[���h�֕\��
	
	auto Screen2d = Manager->GetCurrentScene()->CreateObject(OBJECT_PRESET::OBJECT_RENDER);
	Screen2d->GetRenderer()->Material.albedo = Manager->GetTextureManager()->AddTexture(L"Resource/rhythmFolder/rhythm_Img/Bule_Box.png");
	Screen2d->GetRenderer()->Model = Manager->GetModel()->AddGeometry(MODEL_TYPE_PLAIN);
	Screen2d->GetTransform()->Rotation.x = -90;
	Screen2d->GetTransform()->Position.z += 5;

	for (int i = 0; i < 10; i++)
	{
		auto hit2d = Manager->GetCurrentScene()->CreateObject(OBJECT_PRESET::OBJECT_RENDER);
		hit2d->GetRenderer()->Material.albedo = Manager->GetTextureManager()->AddTexture(L"Resource/rhythmFolder/rhythm_Img/Red_Hit.png");
		hit2d->GetRenderer()->Model = Manager->GetModel()->AddGeometry(MODEL_TYPE_PLAIN);
		hit2d->GetTransform()->Rotation.x = -90;
		hit2d->GetTransform()->Position.z += 5;
		hit2d->GetRenderer()->Enabled = false;
		
		auto _status = Status(hit2d, 1000, false);
		m_NotesStatus.push_back(_status);
	}
}

void RhythmManager::Update(float dt)
{
	auto lastNote = MinStatus(0);
	if (lastNote != nullptr)
	{
		if (lastNote->obj != nullptr && lastNote->active && lastNote->obj->GetRenderer()->Enabled)
		{
			if (m_NotesTiming[lastNote->num] + BAD_TIME*0.5f + 20 < (int)AudioManager::GetInstance().GetControlledMusic()->getPlayPosition())
			{
				lastNote->active = false;
				lastNote->obj->GetRenderer()->Enabled = false;
				ElixirLog("MISS");
			}
		}
	}

	//Status _minStatus = MinStatus(0);
	//ElixirLog(std::to_string((int)AudioManager::GetInstance().GetControlledMusic()->getPlayPosition()));
	for (auto& _status : m_NotesStatus)
	{
		if (_status.active)
		{
			//�ŏI�X�P�[�� + ((�^�C�~���O - ���݂̍Đ�����) * �����X�P�[��) / 1000
			auto _scale = 0.9f + ((m_NotesTiming[_status.num] - (int)AudioManager::GetInstance().GetControlledMusic()->getPlayPosition()) * m_DefaultScale ) /1000;
			_status.obj->GetTransform()->Scale = Vec3f(_scale, 1, _scale);
		}		
	}

	static bool isF5Pressed = false;
	if (GetAsyncKeyState(VK_F5) & 0x8000)
	{
		if (!isF5Pressed)
		{
			isF5Pressed = true;
			for (auto status : m_NotesStatus)
			{
				status.active = false;
				status.obj->GetRenderer()->Enabled = false;
				status.num = 1000;
			}
			m_TimingCount = 0;
			AudioManager::GetInstance().GetControlledMusic()->setPlayPosition(0);
			AudioManager::GetInstance().GetControlledMusic()->setIsPaused(false);
			
		}
	}
	else
	{
		isF5Pressed = false;
	}

	//�L�[���͔���
	if (GetAsyncKeyState(VK_UP)&0x8000)
	{
		if (!m_Press)
		{
			AudioManager::GetInstance().GetControlledMusic()->setIsPaused(false);

			m_Press = true;
			


			HitTimingCheck(MinStatus(0));

			//ElixirLog("Press");
		}
	}
	else 
		m_Press = false;

	if (m_TimingCount >= m_NotesTiming.size())return;
	
	if (m_NotesTiming[m_TimingCount] - 1000 < (int)AudioManager::GetInstance().GetControlledMusic()->getPlayPosition())
	{
		for (auto& _status : m_NotesStatus)
		{
			if (!_status.active)
			{
				//if (m_NotesLaneNumber[m_TimingCount] != 0)break;
				_status.active = true;
				_status.num = m_TimingCount;
				_status.obj->GetRenderer()->Enabled = true;
				_status.obj->GetTransform()->Scale = Vec3f(m_DefaultScale + 1, 1, m_DefaultScale + 1);
				break;
			}
		}
		m_TimingCount++;
	}

	//Make it replayable
	//static bool reloadedSong = false;
	if (AudioManager::GetInstance().GetControlledMusic()->isFinished())
	{
		AudioManager::GetInstance().AddControlledMusic("Resource/rhythmFolder/Dash.mp3");
		AudioManager::GetInstance().GetControlledMusic()->setIsPaused(true);
		AudioManager::GetInstance().GetControlledMusic()->setPlayPosition(0);

		ElixirLog("Ended!");
	}
}

void RhythmManager::HitTimingCheck(Status* _status)
{
	if (_status == nullptr)
		return;

	if (_status->obj == nullptr)
		return;

	int timing = abs(m_NotesTiming[_status->num] - (int)AudioManager::GetInstance().GetControlledMusic()->getPlayPosition());

	//�Y���Aoffset�@���؁B
	if (timing <= GREAT_TIME / 2)
	{
		_status->active = false;
		_status->obj->GetRenderer()->Enabled = false;
		ElixirLog("GREAT");

	}
	else if (timing <= GOOD_TIME / 2)
	{
		_status->active = false;
		_status->obj->GetRenderer()->Enabled = false;
		ElixirLog("GOOD");
	}
	else if (timing <= BAD_TIME / 2)
	{
		_status->active = false;
		_status->obj->GetRenderer()->Enabled = false;
		ElixirLog("BAD");
	}
	else
	{
		_status->active = false;
		_status->obj->GetRenderer()->Enabled = false;
		ElixirLog("MISS");
	}
}
//�e���[���̍ŏ��l�����\���̎擾�֐�
RhythmManager::Status* RhythmManager::MinStatus(int n)
{
	Status* val = nullptr;
	for (auto &status : m_NotesStatus)
	{

		if (status.active)
		{
			//if (m_NotesLaneNumber[status.num] == n)
			{
				if (val == nullptr)
				{
					val = &status;
					continue;
				}
				if (status.num <= val->num)
					val = &status;
			}
		}
	}
	return val;
};