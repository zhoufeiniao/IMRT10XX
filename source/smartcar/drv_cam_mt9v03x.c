﻿#include"drv_cam_mt9v03x.h"
enum cam_mt9v034_reg_t
{
	MT9V034_REG_ChipVersion = 0x00U,
	MT9V034_REG_ChipControll = 0x07U,
	MT9V034_REG_Reset = 0x0CU,
	MT9V034_REG_SensorTypeHdrEn = 0x0FU,
	MT9V034_REG_LedOutCtrl = 0x1BU,
	MT9V034_REG_AdcResolutionControll = 0x1CU,
	MT9V034_REG_RowNoiseCorrectionControll1 = 0x70U,
	MT9V034_REG_TiledDigitalGainBegin = 0x80U,
	MT9V034_REG_TiledDigitalGainEnd = 0x98U,
	MT9V034_REG_AecAgcEnable = 0xAFU,

	MT9V034_REG_ColumnStartContextA = 0x01U,
	MT9V034_REG_RowStartContextA = 0x02U,
	MT9V034_REG_WindowHeightContextA = 0x03U,
	MT9V034_REG_WindowWidthC0ntextA = 0x04U,
	MT9V034_REG_HorizontalBlankingContextA = 0x05U,
	MT9V034_REG_VerticalBlankingContextA = 0x06U,
	MT9V034_REG_CoarseShutterWidth1ContextA = 0x08U,
	MT9V034_REG_CoarseShutterWidth2ContextA = 0x09U,
	MT9V034_REG_CoarseShutterWidthControllContextA = 0x0AU,
	MT9V034_REG_CoarseShutterWidthTotalContextA = 0x0BU,
	MT9V034_REG_FineShutterWidth1ContextA = 0xD3U,
	//MT9V034_REG_FineShutterWidth1ContextA = 0xD4U,
	MT9V034_REG_FineShutterWidthTotalContextA = 0xD5U,
	MT9V034_REG_ReadModeContextA = 0x0DU,
	MT9V034_REG_V1ControllContextA = 0x31U,
	MT9V034_REG_V2ControllContextA = 0x32U,
	MT9V034_REG_V3ControllContextA = 0x33U,
	MT9V034_REG_V4VontrollContextA = 0x34U,
	MT9V034_REG_AnalogGainControllContextA = 0x35U,

	MT9V034_REG_ColumnStartContextB = 0xC9U,
	MT9V034_REG_RowStartContextB = 0xCAU,
	MT9V034_REG_WindowHeightContextB = 0xCBU,
	MT9V034_REG_WindowWidthC0ntextB = 0xCCU,
	MT9V034_REG_HorizontalBlankingContextB = 0xCDU,
	MT9V034_REG_VerticalBlankingContextB = 0xCEU,
	MT9V034_REG_CoarseShutterWidth1ContextB = 0xCFU,
	MT9V034_REG_CoarseShutterWidth2ContextB = 0xD0U,
	MT9V034_REG_CoarseShutterWidthControllContextB = 0xD1U,
	MT9V034_REG_CoarseShutterWidthTotalContextB = 0xD2U,
	MT9V034_REG_FineShutterWidth1ContextB = 0xD6U,
	//MT9V034_REG_FineShutterWidth1ContextB = 0xD7U,
	MT9V034_REG_FineShutterWidthTotalContextB = 0xD8U,
	MT9V034_REG_ReadModeContextB = 0x0EU,
	MT9V034_REG_V1ControllContextB = 0x39U,
	MT9V034_REG_V2ControllContextB = 0x3AU,
	MT9V034_REG_V3ControllContextB = 0x3BU,
	MT9V034_REG_V4VontrollContextB = 0x3CU,
	MT9V034_REG_AnalogGainControllContextB = 0x36U,

};
csi_config_t mt9v034_csi_config = {
	.width = 0,
	.height = 0,
	.polarityFlags = kCSI_HsyncActiveHigh | kCSI_DataLatchOnRisingEdge | kCSI_VsyncActiveHigh,
	.bytesPerPixel = 1,
	.linePitch_Bytes = 0,
	.workMode = kCSI_GatedClockMode,
	.dataBus = kCSI_DataBus8Bit,
	.useExtVsync = true,
};
//image size = 188*120

status_t MT9V034_DataInit(I2CS_Type* base)
{
	mt9v034_csi_config.width = 184;
	mt9v034_csi_config.height = 120;
	mt9v034_csi_config.linePitch_Bytes = mt9v034_csi_config.width * mt9v034_csi_config.bytesPerPixel;
	CAMERA_ReceiverInit(&mt9v034_csi_config);

	_mt9v03x_i2cs = base;
	status_t status = kStatus_Success;
	uint16_t data;
	status = CAM_MT9V03X_I2C_RxWord(MT9V034_REG_ChipVersion, &data);
	if (status != kStatus_Success)
		return status;
	if (data == 0U)
		return kStatus_Fail;

	{
		data = 8U; ///< set window start on 0 column on ContextA
		status = CAM_MT9V03X_I2C_TxWord(MT9V034_REG_ColumnStartContextA, &data);
		data = 0U; ///< set window start on 0 row on ContextA
		status = CAM_MT9V03X_I2C_TxWord(MT9V034_REG_RowStartContextA, &data);
		data = 4U * 120U; ///< set 480 pixel window height on ContextA
		status = CAM_MT9V03X_I2C_TxWord(MT9V034_REG_WindowHeightContextA, &data);
		data = 4U * 184U; ///< set 752 pixel window width on ContextA
		status = CAM_MT9V03X_I2C_TxWord(MT9V034_REG_WindowWidthC0ntextA, &data);
		data = 8U; ///< set 8 pixel clock HorizontalBlanking on ContextA
		status = CAM_MT9V03X_I2C_TxWord(MT9V034_REG_HorizontalBlankingContextA, &data);
		data = 8U; ///< set 8 hsync clock VerticlaBlanking on ContextA
		status = CAM_MT9V03X_I2C_TxWord(MT9V034_REG_VerticalBlankingContextA, &data);

		data = 0b1111U; ///< set 4x4 row & column binning on ContextA
		status = CAM_MT9V03X_I2C_TxWord(MT9V034_REG_ReadModeContextA, &data);
		data = 0x0001U; ///< enable HDR mode on ContextA
		status = CAM_MT9V03X_I2C_TxWord(MT9V034_REG_SensorTypeHdrEn, &data);
		data = 16U; ///< Analog Gain Setting on ContextA. G = 0.0625 * REG, range 0~63.
		status = CAM_MT9V03X_I2C_TxWord(MT9V034_REG_AnalogGainControllContextA, &data);

	}
	if (status != kStatus_Success)
		return status;

	//Official Optimizations
	{
		data = 0x03C7U;
		status = CAM_MT9V03X_I2C_TxWord(0x20U, &data);
		data = 0x001BU;
		status = CAM_MT9V03X_I2C_TxWord(0x24U, &data);
		data = 0x0003U;
		status = CAM_MT9V03X_I2C_TxWord(0x2BU, &data);
		data = 0x0003U;
		status = CAM_MT9V03X_I2C_TxWord(0x2FU, &data);
	}

	return status;
}