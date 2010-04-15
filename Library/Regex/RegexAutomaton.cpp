#include "RegexAutomaton.h"
#include "..\Collections\OperationCopyFrom.h"

namespace vl
{
	namespace regex_internal
	{

/***********************************************************************
Automaton
***********************************************************************/
		
		Automaton::Automaton()
		{
			startState=0;
		}

		State* Automaton::NewState()
		{
			State* state=new State;
			state->finalState=false;
			state->userData=0;
			states.Add(state);
			return state;
		}

		Transition* Automaton::NewTransition(State* start, State* end)
		{
			Transition* transition=new Transition;
			transition->source=start;
			transition->target=end;
			start->transitions.Add(transition);
			end->inputs.Add(transition);
			transitions.Add(transition);
			return transition;
		}

		Transition* Automaton::NewChars(State* start, State* end, CharRange range)
		{
			Transition* transition=NewTransition(start, end);
			transition->type=Transition::Chars;
			transition->range=range;
			return transition;
		}

		Transition* Automaton::NewEpsilon(State* start, State* end)
		{
			Transition* transition=NewTransition(start, end);
			transition->type=Transition::Epsilon;
			return transition;
		}

		Transition* Automaton::NewBeginString(State* start, State* end)
		{
			Transition* transition=NewTransition(start, end);
			transition->type=Transition::BeginString;
			return transition;
		}

		Transition* Automaton::NewEndString(State* start, State* end)
		{
			Transition* transition=NewTransition(start, end);
			transition->type=Transition::EndString;
			return transition;
		}

		Transition* Automaton::NewNop(State* start, State* end)
		{
			Transition* transition=NewTransition(start, end);
			transition->type=Transition::Nop;
			return transition;
		}

		Transition* Automaton::NewCapture(State* start, State* end, int capture)
		{
			Transition* transition=NewTransition(start, end);
			transition->type=Transition::Capture;
			transition->capture=capture;
			return transition;
		}

		Transition* Automaton::NewMatch(State* start, State* end, int capture, int index)
		{
			Transition* transition=NewTransition(start, end);
			transition->type=Transition::Match;
			transition->capture=capture;
			transition->index=index;
			return transition;
		}

		Transition* Automaton::NewPositive(State* start, State* end)
		{
			Transition* transition=NewTransition(start, end);
			transition->type=Transition::Positive;
			return transition;
		}

		Transition* Automaton::NewNegative(State* start, State* end)
		{
			Transition* transition=NewTransition(start, end);
			transition->type=Transition::Negative;
			return transition;
		}

		Transition* Automaton::NewNegativeFail(State* start, State* end)
		{
			Transition* transition=NewTransition(start, end);
			transition->type=Transition::NegativeFail;
			return transition;
		}

		Transition* Automaton::NewEnd(State* start, State* end)
		{
			Transition* transition=NewTransition(start, end);
			transition->type=Transition::End;
			return transition;
		}

/***********************************************************************
��������
***********************************************************************/

		bool PureEpsilonChecker(Transition* transition)
		{
			switch(transition->type)
			{
			case Transition::Epsilon:
			case Transition::Nop:
			case Transition::Capture:
			case Transition::End:
				return true;
			default:
				return false;
			}
		}

		bool RichEpsilonChecker(Transition* transition)
		{
			switch(transition->type)
			{
			case Transition::Epsilon:
				return true;
			default:
				return false;
			}
		}

		bool AreEqual(Transition* transA, Transition* transB)
		{
			if(transA->type!=transB->type)return false;
			switch(transA->type)
			{
			case Transition::Chars:
				return transA->range==transB->range;
			case Transition::Capture:
				return transA->capture==transB->capture;
			case Transition::Match:
				return transA->capture==transB->capture && transA->index==transB->index;
			default:
				return true;
			}
		}

		//�ݹ鱣֤ת���Ⱥ�˳��
		void CollectEpsilon(State* targetState, State* sourceState, bool(*epsilonChecker)(Transition*), List<State*>& epsilonStates, List<Transition*>& transitions)
		{
			if(!epsilonStates.Contains(sourceState))
			{
				epsilonStates.Add(sourceState);
				for(int i=0;i<sourceState->transitions.Count();i++)
				{
					Transition* transition=sourceState->transitions[i];
					if(epsilonChecker(transition))
					{
						if(!epsilonStates.Contains(transition->target))
						{
							if(transition->target->finalState)
							{
								targetState->finalState=true;
							}
							CollectEpsilon(targetState, transition->target, epsilonChecker, epsilonStates, transitions);
						}
					}
					else
					{
						transitions.Add(transition);
					}
				}
			}
		}

		Automaton::Ref EpsilonNfaToNfa(Automaton::Ref source, bool(*epsilonChecker)(Transition*), IDictionary<State*, State*>& nfaStateMap)
		{
			Automaton::Ref target=new Automaton;
			Dictionary<State*, State*> stateMap;	//source->target
			List<State*> epsilonStates;				//ÿ�ε�����ǰ״̬��epsilon�հ�
			List<Transition*> transitions;			//ÿ�ε�����ǰ״̬��epsilon�հ���ת������

			stateMap.Add(source->startState, target->NewState());
			nfaStateMap.Add(stateMap[source->startState], source->startState);
			target->startState=target->states[0].Obj();
			CopyFrom(target->captureNames.Wrap(), source->captureNames.Wrap());

			for(int i=0;i<target->states.Count();i++)
			{
				//���epsilonStates�������Լ�
				State* targetState=target->states[i].Obj();
				State* sourceState=nfaStateMap[targetState];
				if(sourceState->finalState)
				{
					targetState->finalState=true;
				}
				epsilonStates.Clear();
				transitions.Clear();

				//�����в�����epsilonStates���б������������״̬��һ��epsilonֱ��Ŀ��ӽ�ȥ������������
				CollectEpsilon(targetState, sourceState, epsilonChecker, epsilonStates, transitions);

				//��������epsilon�հ�ת��
				for(int j=0;j<transitions.Count();j++)
				{
					Transition* transition=transitions[j];
					//Ѱ�ҵ�һ����epsilon�հ���ʱ�����ӳ��
					if(!stateMap.Keys().Contains(transition->target))
					{
						stateMap.Add(transition->target, target->NewState());
						nfaStateMap.Add(stateMap[transition->target], transition->target);
					}
					//����ת�����Ƶ���״̬����
					Transition* newTransition=target->NewTransition(targetState, stateMap[transition->target]);
					newTransition->capture=transition->capture;
					newTransition->index=transition->index;
					newTransition->range=transition->range;
					newTransition->type=transition->type;
				}
			}
			return target;
		}

		Automaton::Ref NfaToDfa(Automaton::Ref source, IGroup<State*, State*>& dfaStateMap)
		{
			Automaton::Ref target=new Automaton;
			Group<Transition*, Transition*> nfaTransitions;
			List<Transition*> transitionClasses;//��֤ת���Ⱥ�˳�򲻱�nfaTransitions.Keys�ƻ�

			CopyFrom(target->captureNames.Wrap(), source->captureNames.Wrap());
			State* startState=target->NewState();
			target->startState=startState;
			dfaStateMap.Add(startState, source->startState);

			for(int i=0;i<target->states.Count();i++)
			{
				State* currentState=target->states[i].Obj();
				nfaTransitions.Clear();
				transitionClasses.Clear();

				//�Ը�DFA״̬�����еȼ�NFA״̬���б���
				const IReadonlyList<State*>& nfaStates=dfaStateMap[currentState];
				for(int j=0;j<nfaStates.Count();j++)
				{
					State* nfaState=nfaStates[j];
					//��ÿһ��NFA״̬������ת�����б���
					for(int k=0;k<nfaState->transitions.Count();k++)
					{
						Transition* nfaTransition=nfaState->transitions[k];
						//����NFAת�������Ƿ��Ѿ������Ѿ�����¼
						Transition* transitionClass=0;
						for(int l=0;l<nfaTransitions.Keys().Count();l++)
						{
							Transition* key=nfaTransitions.Keys()[l];
							if(AreEqual(key, nfaTransition))
							{
								transitionClass=key;
								break;
							}
						}
						//�������򴴽�һ��ת������
						if(transitionClass==0)
						{
							transitionClass=nfaTransition;
							transitionClasses.Add(transitionClass);
						}
						//ע��ת��
						nfaTransitions.Add(transitionClass, nfaTransition);
					}
				}

				SortedList<State*> transitionTargets;
				SortedList<State*> relativeStates;
				transitionTargets.SetLessMemoryMode(false);
				relativeStates.SetLessMemoryMode(false);
				//�������������NFAת��
				for(int j=0;j<transitionClasses.Count();j++)
				{
					const IReadonlyList<Transition*>& transitionSet=nfaTransitions[transitionClasses[j]];
					//������ת����NFAĿ��״̬���Ͻ�������
					transitionTargets.Clear();
					for(int l=0;l<transitionSet.Count();l++)
					{
						State* nfaState=transitionSet[l]->target;
						if(!transitionTargets.Contains(nfaState))
						{
							transitionTargets.Add(nfaState);
						}
					}
					//�ж�ת���������ת����NFAĿ��״̬��ɵļ����Ƿ��Ѿ���һ����Ӧ��DFA״̬
					State* dfaState=0;
					for(int k=0;k<dfaStateMap.Count();k++)
					{
						//��DFA�ĵȼ�NFA״̬���Ͻ�������
						relativeStates.Clear();
						CopyFrom(relativeStates.Wrap(), dfaStateMap.GetByIndex(k));
						//�Ƚ������Ƿ����
						if(relativeStates.Count()==transitionTargets.Count())
						{
							bool equal=true;
							for(int l=0;l<relativeStates.Count();l++)
							{
								if(relativeStates[l]!=transitionTargets[l])
								{
									equal=false;
									break;
								}
							}
							if(equal)
							{
								dfaState=dfaStateMap.Keys()[k];
								break;
							}
						}
					}
					//�����ڵȼ�DFA״̬�򴴽�һ��
					if(!dfaState)
					{
						dfaState=target->NewState();
						for(int k=0;k<transitionTargets.Count();k++)
						{
							dfaStateMap.Add(dfaState, transitionTargets[k]);
							if(transitionTargets[k]->finalState)
							{
								dfaState->finalState=true;
							}
						}
					}
					//����ת�����Ƶ���״̬����
					Transition* transitionClass=transitionClasses[j];
					Transition* newTransition=target->NewTransition(currentState, dfaState);
					newTransition->capture=transitionClass->capture;
					newTransition->index=transitionClass->index;
					newTransition->range=transitionClass->range;
					newTransition->type=transitionClass->type;
				}
			}

			return target;
		}
	}
}