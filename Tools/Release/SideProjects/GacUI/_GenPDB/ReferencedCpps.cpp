#include "..\..\..\..\..\Candidate\GUI\GUI\Controls\GuiApplication.cpp"
#include "..\..\..\..\..\Candidate\GUI\GUI\Controls\GuiBasicControls.cpp"
#include "..\..\..\..\..\Candidate\GUI\GUI\Controls\GuiListControls.cpp"
#include "..\..\..\..\..\Candidate\GUI\GUI\Controls\GuiTextControls.cpp"
#include "..\..\..\..\..\Candidate\GUI\GUI\Controls\GuiWindowControls.cpp"
//---------------------------------------------------------------
#include "..\..\..\..\..\Candidate\GUI\GUI\Controls\ExtendedControls\GuiComboControls.cpp"
#include "..\..\..\..\..\Candidate\GUI\GUI\Controls\ExtendedControls\GuiContainerControls.cpp"
#include "..\..\..\..\..\Candidate\GUI\GUI\Controls\ExtendedControls\GuiListViewControls.cpp"
#include "..\..\..\..\..\Candidate\GUI\GUI\Controls\ExtendedControls\GuiMenuControls.cpp"
#include "..\..\..\..\..\Candidate\GUI\GUI\Controls\ExtendedControls\GuiTextListControls.cpp"
#include "..\..\..\..\..\Candidate\GUI\GUI\Controls\ExtendedControls\GuiTreeViewControls.cpp"
//---------------------------------------------------------------
#include "..\..\..\..\..\Candidate\GUI\GUI\Controls\Styles\GuiCommonStyles.cpp"
#include "..\..\..\..\..\Candidate\GUI\GUI\Controls\Styles\GuiWin7Styles.cpp"
//---------------------------------------------------------------
#include "..\..\..\..\..\Candidate\GUI\GUI\GraphicsElement\GuiGraphicsComposition.cpp"
#include "..\..\..\..\..\Candidate\GUI\GUI\GraphicsElement\GuiGraphicsElement.cpp"
#include "..\..\..\..\..\Candidate\GUI\GUI\GraphicsElement\GuiGraphicsEventReceiver.cpp"
#include "..\..\..\..\..\Candidate\GUI\GUI\GraphicsElement\GuiGraphicsHost.cpp"
#include "..\..\..\..\..\Candidate\GUI\GUI\GraphicsElement\GuiGraphicsTextElement.cpp"
#include "..\..\..\..\..\Candidate\GUI\GUI\GraphicsElement\WindowsGDI\GuiGraphicsWindowsGDI.cpp"
#include "..\..\..\..\..\Candidate\GUI\GUI\GraphicsElement\WindowsGDI\GuiGraphicsRenderersWindowsGDI.cpp"
#include "..\..\..\..\..\Candidate\GUI\GUI\GraphicsElement\WindowsDirect2D\GuiGraphicsWindowsDirect2D.cpp"
#include "..\..\..\..\..\Candidate\GUI\GUI\GraphicsElement\WindowsDirect2D\GuiGraphicsRenderersWindowsDirect2D.cpp"
//---------------------------------------------------------------
#include "..\..\..\..\..\Candidate\GUI\GUI\Reflection\GuiTypeDescriptor.cpp"
#include "..\..\..\..\..\Candidate\GUI\GUI\Reflection\GuiTypeDescriptorImpHelper.cpp"
//---------------------------------------------------------------
#include "..\..\..\..\..\Candidate\GUI\GUI\NativeWindow\GuiNativeWindow.cpp"
#include "..\..\..\..\..\Candidate\GUI\GUI\NativeWindow\Windows\WinNativeWindow.cpp"
#include "..\..\..\..\..\Candidate\GUI\GUI\NativeWindow\Windows\Direct2D\WinDirect2DApplication.cpp"
#include "..\..\..\..\..\Candidate\GUI\GUI\NativeWindow\Windows\GDI\WinGDIApplication.cpp"
#include "..\..\..\..\..\Candidate\GUI\GUI\NativeWindow\Windows\GDI\WinGDI.cpp"
//---------------------------------------------------------------
#include "..\..\..\..\..\Library\Basic.cpp"
#include "..\..\..\..\..\Library\Exception.cpp"
#include "..\..\..\..\..\Library\String.cpp"
#include "..\..\..\..\..\Library\Threading.cpp"
#include "..\..\..\..\..\Library\Collections\Operation.cpp"
//#include "..\..\..\..\..\Library\Stream\Accessor.cpp"
//#include "..\..\..\..\..\Library\Stream\BroadcastStream.cpp"
//#include "..\..\..\..\..\Library\Stream\CacheStream.cpp"
//#include "..\..\..\..\..\Library\Stream\CharFormat.cpp"
#include "..\..\..\..\..\Library\Stream\FileStream.cpp"
//#include "..\..\..\..\..\Library\Stream\MemoryStream.cpp"
//#include "..\..\..\..\..\Library\Stream\MemoryWrapperStream.cpp"
//#include "..\..\..\..\..\Library\Stream\RecorderStream.cpp"
//#include "..\..\..\..\..\Library\Entity\TreeJson.cpp"
//#include "..\..\..\..\..\Library\Entity\TreeQuery.cpp"
//#include "..\..\..\..\..\Library\Entity\TreeXml.cpp"

void GuiNotifyEventHandler(vl::presentation::elements::GuiGraphicsComposition*, vl::presentation::elements::GuiEventArgs&){}
void GuiRequestEventHandler(vl::presentation::elements::GuiGraphicsComposition*, vl::presentation::elements::GuiRequestEventArgs&){}
void GuiKeyEventEventHandler(vl::presentation::elements::GuiGraphicsComposition*, vl::presentation::elements::GuiKeyEventArgs&){}
void GuiCharEventEventHandler(vl::presentation::elements::GuiGraphicsComposition*, vl::presentation::elements::GuiCharEventArgs&){}
void GuiMouseEventEventHandler(vl::presentation::elements::GuiGraphicsComposition*, vl::presentation::elements::GuiMouseEventArgs&){}

namespace fillabstractmethods
{
	void ClassNewer()
	{
		vl::presentation::elements::GuiEventArgs(0);
		vl::presentation::elements::GuiRequestEventArgs(0);
		vl::presentation::elements::GuiKeyEventArgs(0);
		vl::presentation::elements::GuiCharEventArgs(0);
		vl::presentation::elements::GuiMouseEventArgs(0);

		vl::presentation::elements::GuiEventArgs();
		vl::presentation::elements::GuiRequestEventArgs();
		vl::presentation::elements::GuiKeyEventArgs();
		vl::presentation::elements::GuiCharEventArgs();
		vl::presentation::elements::GuiMouseEventArgs();

		vl::presentation::elements::GuiNotifyEvent().AttachFunction(&GuiNotifyEventHandler);
		vl::presentation::elements::GuiRequestEvent().AttachFunction(&GuiRequestEventHandler);
		vl::presentation::elements::GuiKeyEvent().AttachFunction(&GuiKeyEventEventHandler);
		vl::presentation::elements::GuiCharEvent().AttachFunction(&GuiCharEventEventHandler);
		vl::presentation::elements::GuiMouseEvent().AttachFunction(&GuiMouseEventEventHandler);

		vl::presentation::elements::GuiNotifyEvent().Detach(0);
		vl::presentation::elements::GuiRequestEvent().Detach(0);
		vl::presentation::elements::GuiKeyEvent().Detach(0);
		vl::presentation::elements::GuiCharEvent().Detach(0);
		vl::presentation::elements::GuiMouseEvent().Detach(0);

		vl::presentation::elements::GuiNotifyEvent().SetAssociatedComposition(0);
		vl::presentation::elements::GuiRequestEvent().SetAssociatedComposition(0);
		vl::presentation::elements::GuiKeyEvent().SetAssociatedComposition(0);
		vl::presentation::elements::GuiCharEvent().SetAssociatedComposition(0);
		vl::presentation::elements::GuiMouseEvent().SetAssociatedComposition(0);

		vl::presentation::controls::list::TextItemProvider().NotifyUpdate(0);
		vl::presentation::Rect().Expand(0, 0);
		vl::presentation::Rect().Expand(vl::presentation::Size(0, 0));
		vl::presentation::Rect().Move(0, 0);
		vl::presentation::Rect().Move(vl::presentation::Size(0, 0));

		vl::collections::List<vl::WString> strings;
		strings.Contains(L"");
		strings.IndexOf(L"");
		strings.Add(L"");
		strings.Insert(0, L"");
		strings.Remove(L"");
		strings.Set(0, L"");
	}
}