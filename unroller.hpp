#ifndef MFL_UNROLLER_INCLUDED
#define MFL_UNROLLER_INCLUDED


namespace MFL{
#pragma warning(push)
#pragma warning(disable:4100)
	template<int Begin, int End, int Step = 1>
	struct Unroller {
		template<typename Action>
		static void step(Action& action) {
			action(Begin);
			Unroller<Begin+Step, End, Step>::step(action);
		}
	};

	template<int End, int Step>
	struct Unroller<End, End, Step> {
		template<typename Action>
		static void step(Action& action) {
		}
	};
#pragma warning(pop)
};

#endif //MFL_UNROLLER_INCLUDED