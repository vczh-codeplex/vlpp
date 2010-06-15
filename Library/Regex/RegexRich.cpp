#include "RegexRich.h"

namespace vl
{
	namespace regex_internal
	{

/***********************************************************************
���ݸ������ݽṹ
***********************************************************************/

		class SaverBase
		{
		public:
			bool				available;
			vint					previous;
		};

		class StateSaver
		{
		public:
			enum StateStoreType
			{
				Positive,
				Negative,
				Other
			};

			const wchar_t*		reading;					//��ǰ�ַ���λ��
			State*				currentState;				//��ǰ״̬
			vint					minTransition;				//��С����ת��
			vint					captureCount;				//��Чcapture����
			vint					stateSaverCount;			//��Ч����״̬����
			vint					extensionSaverAvailable;	//��Чδ�����չ��������
			vint					extensionSaverCount;		//����δ�����չ��������
			StateStoreType		storeType;					//����״̬��ԭ��

			bool operator==(const StateSaver& saver)const
			{
				return
					reading==saver.reading &&
					currentState==saver.currentState &&
					minTransition==saver.minTransition &&
					captureCount==saver.captureCount;
			}
		};

		class ExtensionSaver : public SaverBase
		{
		public:
			vint					captureListIndex;
			Transition*			transition;
			const wchar_t*		reading;

			bool operator==(const ExtensionSaver& saver)const
			{
				return
					captureListIndex==saver.captureListIndex &&
					transition==saver.transition &&
					reading==saver.reading;
			}
		};

		template<typename T, typename K>
		void Push(List<T, K>& elements, vint& available, vint& count, const T& element)
		{
			if(elements.Count()==count)
			{
				elements.Add(element);
			}
			else
			{
				elements[count]=element;
			}
			T& current=elements[count];
			current.previous=available;
			available=count++;
		}

		template<typename T, typename K>
		T Pop(List<T, K>& elements, vint& available, vint& count)
		{
			T& current=elements[available];
			available=current.previous;
			return current;
		}

		template<typename T, typename K>
		void PushNonSaver(List<T, K>& elements, vint& count, const T& element)
		{
			if(elements.Count()==count)
			{
				elements.Add(element);
			}
			else
			{
				elements[count]=element;
			}
			count++;
		}

		template<typename T, typename K>
		T PopNonSaver(List<T, K>& elements, vint& count)
		{
			return elements[--count];
		}
	}

	template<>
	struct POD<regex_internal::StateSaver>
	{
		static const bool Result=true;
	};

	template<>
	struct POD<regex_internal::ExtensionSaver>
	{
		static const bool Result=true;
	};

	namespace regex_internal
	{
/***********************************************************************
CaptureRecord
***********************************************************************/

		bool CaptureRecord::operator==(const CaptureRecord& record)const
		{
			return capture==record.capture && start==record.start && length==record.length;
		}

/***********************************************************************
RichInterpretor
***********************************************************************/

		RichInterpretor::RichInterpretor(Automaton::Ref _dfa)
			:dfa(_dfa)
		{
			datas=new UserData[dfa->states.Count()];

			for(vint i=0;i<dfa->states.Count();i++)
			{
				State* state=dfa->states[i].Obj();
				vint charEdges=0;
				vint nonCharEdges=0;
				bool mustSave=false;
				for(vint j=0;j<state->transitions.Count();j++)
				{
					if(state->transitions[j]->type==Transition::Chars)
					{
						charEdges++;
					}
					else
					{
						if(state->transitions[j]->type==Transition::Negative ||
						   state->transitions[j]->type==Transition::Positive)
						{
							mustSave=true;
						}
						nonCharEdges++;
					}
				}
				datas[i].NeedKeepState=mustSave || nonCharEdges>1 || (nonCharEdges!=0 && charEdges!=0);
				state->userData=&datas[i];
			}
		}

		RichInterpretor::~RichInterpretor()
		{
			delete[] datas;
		}

		bool RichInterpretor::MatchHead(const wchar_t* input, const wchar_t* start, RichResult& result)
		{
			List<StateSaver> stateSavers;
			List<ExtensionSaver> extensionSavers;

			StateSaver currentState;
			currentState.captureCount=0;
			currentState.currentState=dfa->startState;
			currentState.extensionSaverAvailable=-1;
			currentState.extensionSaverCount=0;
			currentState.minTransition=0;
			currentState.reading=input;
			currentState.stateSaverCount=0;
			currentState.storeType=StateSaver::Other;

			while(!currentState.currentState->finalState)
			{
				bool found=false;
				StateSaver oldState=currentState;
				//��ʼ����ת��
				for(vint i=currentState.minTransition;i<currentState.currentState->transitions.Count();i++)
				{
					Transition* transition=currentState.currentState->transitions[i];
					switch(transition->type)
					{
					case Transition::Chars:
						{
							CharRange range=transition->range;
							found=
								range.begin<=*currentState.reading && 
								range.end>=*currentState.reading;
							if(found)
							{
								currentState.reading++;
							}
						}
						break;
					case Transition::BeginString:
						{
							found=currentState.reading==start;
						}
						break;
					case Transition::EndString:
						{
							found=*currentState.reading==L'\0';
						}
						break;
					case Transition::Nop:
						{
							found=true;
						}
						break;
					case Transition::Capture:
						{
							ExtensionSaver saver;
							saver.captureListIndex=currentState.captureCount;
							saver.reading=currentState.reading;
							saver.transition=transition;
							Push(extensionSavers, currentState.extensionSaverAvailable, currentState.extensionSaverCount, saver);

							CaptureRecord capture;
							capture.capture=transition->capture;
							capture.start=currentState.reading-start;
							capture.length=-1;
							PushNonSaver(result.captures, currentState.captureCount, capture);

							found=true;
						}
						break;
					case Transition::Match:
						{
							vint index=0;
							for(vint j=0;j<currentState.captureCount;j++)
							{
								CaptureRecord& capture=result.captures[j];
								if(capture.capture==transition->capture)
								{
									if(capture.length!=-1 && (transition->index==-1 || transition->index==index))
									{
										if(wcsncmp(start+capture.start, currentState.reading, capture.length)==0)
										{
											currentState.reading+=capture.length;
											found=true;
											break;
										}
									}
									if(transition->index!=-1 && index==transition->index)
									{
										break;
									}
									else
									{
										index++;
									}
								}
							}
						}
						break;
					case Transition::Positive:
						{
							ExtensionSaver saver;
							saver.captureListIndex=-1;
							saver.reading=currentState.reading;
							saver.transition=transition;
							Push(extensionSavers, currentState.extensionSaverAvailable, currentState.extensionSaverCount, saver);
							//Positive��oldStateһ���ᱻpush
							oldState.storeType=StateSaver::Positive;
							found=true;
						}
						break;
					case Transition::Negative:
						{
							ExtensionSaver saver;
							saver.captureListIndex=-1;
							saver.reading=currentState.reading;
							saver.transition=transition;
							Push(extensionSavers, currentState.extensionSaverAvailable, currentState.extensionSaverCount, saver);
							//Negative��oldStateһ���ᱻpush
							oldState.storeType=StateSaver::Negative;
							found=true;
						}
						break;
					case Transition::NegativeFail:
						{
							//ֻ���ڻ��ݵ�ʱ��NegativeFail�Żᱻ����
						}
						break;
					case Transition::End:
						{
							ExtensionSaver extensionSaver=Pop(extensionSavers, currentState.extensionSaverAvailable, currentState.extensionSaverCount);
							switch(extensionSaver.transition->type)
							{
							case Transition::Capture:
								{
									CaptureRecord& capture=result.captures[extensionSaver.captureListIndex];
									capture.length=(currentState.reading-start)-capture.start;
									found=true;
								}
								break;
							case Transition::Positive:
								for(vint j=currentState.stateSaverCount-1;j>=0;j--)
								{
									StateSaver& stateSaver=stateSavers[j];
									if(stateSaver.storeType==StateSaver::Positive)
									{
										oldState.reading=stateSaver.reading;
										oldState.stateSaverCount=j;
										currentState.reading=stateSaver.reading;
										currentState.stateSaverCount=j;
										break;
									}
								}
								found=true;
								break;
							case Transition::Negative:
								for(vint j=currentState.stateSaverCount-1;j>=0;j--)
								{
									StateSaver& stateSaver=stateSavers[j];
									if(stateSaver.storeType==StateSaver::Negative)
									{
										oldState=stateSaver;
										oldState.storeType=StateSaver::Other;
										currentState=stateSaver;
										currentState.storeType=StateSaver::Other;
										i=currentState.minTransition-1;
										break;
									}
								}
								break;
							}
						}
						break;
					}
					//Ѱ�ҳɹ����ڱ�Ҫ��ʱ�򱣴浱ǰ�Ļ���״̬
					if(found)
					{
						UserData* data=(UserData*)currentState.currentState->userData;
						if(data->NeedKeepState)
						{
							oldState.minTransition=i+1;
							PushNonSaver(stateSavers, currentState.stateSaverCount, oldState);
						}
						currentState.currentState=transition->target;
						currentState.minTransition=0;
						break;
					}
				}
				if(!found)
				{
					//���ڻ��ݼ�¼�����
					if(currentState.stateSaverCount)
					{
						//�ָ�Negativeʧ��״̬��ʱ��Ҫ�ƶ���NegativeFail����
						currentState=PopNonSaver(stateSavers, currentState.stateSaverCount);
						//minTransition���Ǳ�+1�󱣴棬���ֱ��-1������Чֵ
						if(currentState.currentState->transitions[currentState.minTransition-1]->type==Transition::Negative)
						{
							//Ѱ��NegativeFail
							for(vint i=0;i<currentState.currentState->transitions.Count();i++)
							{
								Transition* transition=currentState.currentState->transitions[i];
								if(transition->type==Transition::NegativeFail)
								{
									//����ǰ״̬�ƶ���NegativeFail����
									currentState.currentState=transition->target;
									currentState.minTransition=0;
									currentState.storeType=StateSaver::Other;
									break;
								}
							}
						}
					}
					else
					{
						break;
					}
				}
			}

			//�ж��Ƿ�ɹ����Ҵ����ؽ��
			if(currentState.currentState->finalState)
			{
				result.start=input-start;
				result.length=(currentState.reading-start)-result.start;
				for(vint i=result.captures.Count()-1;i>=currentState.captureCount;i--)
				{
					result.captures.RemoveAt(i);
				}
				return true;
			}
			else
			{
				result.captures.Clear();
				return false;
			}
		}

		bool RichInterpretor::Match(const wchar_t* input, const wchar_t* start, RichResult& result)
		{
			const wchar_t* read=input;
			while(*read)
			{
				if(MatchHead(read, start, result))
				{
					return true;
				}
				read++;
			}
			return false;
		}

		const IReadonlyList<WString>& RichInterpretor::CaptureNames()
		{
			return dfa->captureNames.Wrap();
		}
	}
}