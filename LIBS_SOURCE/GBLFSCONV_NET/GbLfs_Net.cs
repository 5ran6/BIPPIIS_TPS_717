using System;
using System.Runtime.InteropServices;
using GBLFS_DllWrapper;

/////////////////////////
// VER 2.0.0.0
// new namespace
/////////////////////////
namespace GBLFS_DllWrapper
{
    /************************************************************
     ************************************************************
     *
     * Globals definition
     * 
     ************************************************************
    ************************************************************/
    public class LfsConv_Constants
    {
        public const int Success = 0;
        public const int Failure = 1;
    }
    /************************************************************
     ************************************************************
     *
     * Structures definition: PACK 1 for compatibility with
     * C structures
     * 
     ************************************************************
    ************************************************************/
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct MINUTIA
    {
        public int x;
        public int y;
        public int ex;
        public int ey;
        public int direction;
        public double reliability;
        public int type;
        public int appearing;
        public int feature_id;
        public IntPtr nbrs; // int*
        public IntPtr ridge_counts; // int*
        public int num_nbrs;
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct MINUTIAE
    {
        public int alloc;
        public int num;
        public IntPtr list; // MINUTIA**
    }

    /************************************************************
     ************************************************************
     *
     * Direct dll wrapper
     * 
     ************************************************************
    ************************************************************/
    public class LfsConvdll_NetWrapper
    {
        [DllImport("LfsConv.dll", EntryPoint = "FMRMinutiaeToLfs")]
        public static extern int FMRMinutiaeToLfs(
            IntPtr FMR, // unsigned char *
            Byte FormatType,
            IntPtr Minutiae //MINUTIAE **
        );

        [DllImport("LfsConv.dll", EntryPoint = "FMCMinutiaeToLfs")]
        public static extern int FMCMinutiaeToLfs(
            IntPtr FMC, // unsigned char * 
            UInt16 FMCLength,
            Byte FormatType,
            IntPtr Minutiae //MINUTIAE **
        );

        [DllImport("LfsConv.dll", EntryPoint = "FreeMinutiae", CharSet = CharSet.Ansi)]
        public static extern void FreeMinutiae(
            IntPtr minutiae // MINUTIAE **
        );
    }
    /************************************************************
     ************************************************************
     *
     * Bozorth Direct dll wrapper
     * 
     ************************************************************
    ************************************************************/
    public class Bozorthdll_NetWrapper
    {
        [DllImport("Bozorth.dll", EntryPoint = "boz_direct_call")]
        public static extern int boz_direct_call(
            int maxminutiae,
            ref MINUTIAE probe, // MINUTIAE *
            ref MINUTIAE gallery // MINUTIAE *
        );
    }
}


namespace GBLFS_NET
{
    public class GBLFS_NET_Error
	{
        public static int Success = 1;
        public static int Failure = 0;
        /// <summary>
        /// left for backward compatibility:
        /// no more used
        /// </summary>
        public static int Exception = 255;
        /// <summary>
        /// left for backward compatibility:
        /// no more used
        /// </summary>
		public static String ExceptionText;
	};

    public class GBLFS_NET_Minutia
	{
        public int x;
        public int y;
		// reserved
        public int ex;
		// reserved
        public int ey;
        public int direction;
        public double reliability;
        public int type;
		// reserved
        public int appearing;
		// reserved
        public int feature_id;
		// reserved
        public IntPtr nbrs;
		// reserved
        public IntPtr ridge_counts;
		// reserved
        public int num_nbrs;

        /// <summary>
        /// VER 2.0.0.0
        /// </summary>
        /// <param name="i_x"></param>
        /// <param name="i_y"></param>
        /// <param name="i_ex"></param>
        /// <param name="i_ey"></param>
        /// <param name="i_direction"></param>
        /// <param name="i_reliability"></param>
        /// <param name="i_type"></param>
        /// <param name="i_appearing"></param>
        /// <param name="i_feature_id"></param>
        /// <param name="i_nbrs"></param>
        /// <param name="i_ridge_c"></param>
        /// <param name="i_num_nbrs"></param>
        public GBLFS_NET_Minutia(
            int i_x,
            int i_y,
            int i_ex,
            int i_ey,
            int i_direction,
            double i_reliability,
            int i_type,
            int i_appearing,
            int i_feature_id,
            IntPtr i_nbrs,
            IntPtr i_ridge_c,
            int i_num_nbrs
        )
        {
            this.x = i_x;
            this.y = i_y;
            this.ex = i_ex;
            this.ey = i_ey;
            this.direction = i_direction;
            this.reliability = i_reliability;
            this.type = i_type;
            this.appearing = i_appearing;
            this.feature_id = i_feature_id;
            this.nbrs = i_nbrs;
            this.ridge_counts = i_ridge_c;
            this.num_nbrs = i_num_nbrs;
        }

        public GBLFS_DllWrapper.MINUTIA ConvertToLowLevelMinutia()
        {
            MINUTIA LLItem = new MINUTIA();

            LLItem.appearing = this.appearing;
            LLItem.direction = this.direction;
            LLItem.ex = this.ex;
            LLItem.ey = this.ey;
            LLItem.feature_id = this.feature_id;
            LLItem.nbrs = this.nbrs;
            LLItem.num_nbrs = this.num_nbrs;
            LLItem.reliability = this.reliability;
            LLItem.ridge_counts = this.ridge_counts;
            LLItem.type = this.type;
            LLItem.x = this.x;
            LLItem.y = this.y;

            return LLItem;
        }
	};

    public class GBLFS_NET_Functions
	{
		////////////////////////////////////////
		// MINUTIAE EXTRACTION
		////////////////////////////////////////
		/*********************
		** FMR
		*********************/
        public static Int32 GBLFS_NET_ExtractMinutiae(
            Byte[] FMR,
            Byte FormatType,
            out GBLFS_NET_Minutia [] Minutiae
        )
        {
            try
            {
                Minutiae = null;

                ////////////////////////////////
                // Prepare Minutiae list pointer
                ////////////////////////////////
                IntPtr pMinList = Marshal.AllocHGlobal(IntPtr.Size);
                if (pMinList == IntPtr.Zero)
                {
                    Minutiae = null;
                    throw new Exception("GBLFS_NET_ExtractMinutiae: cannot alloc pMinList");
                }
                ////////////////////////////////
                // Prepare FMR buffer pointer
                ////////////////////////////////
                IntPtr FmrBuffer = Marshal.AllocHGlobal(FMR.Length);
                if (pMinList == IntPtr.Zero)
                {
                    Minutiae = null;
                    Marshal.FreeHGlobal(pMinList);
                    throw new Exception("GBLFS_NET_ExtractMinutiae: cannot alloc FmrBuffer");
                }
                Marshal.Copy(FMR, 0, FmrBuffer, FMR.Length);

                ////////////////////////////////
                // Call low level function
                ////////////////////////////////
                int RetVal = LfsConvdll_NetWrapper.FMRMinutiaeToLfs(FmrBuffer, FormatType, pMinList);
                // free FMR buffer
                Marshal.FreeHGlobal(FmrBuffer);
                // check low level return value
                if (RetVal != LfsConv_Constants.Success)
                {
                    Marshal.FreeHGlobal(pMinList);
                    Minutiae = null;
                    return RetVal;
                }

                ////////////////////////////////
                // Now get minutiae list pointer
                ////////////////////////////////
                IntPtr minlist = Marshal.ReadIntPtr(pMinList);
                if (minlist == IntPtr.Zero)
                {
                    Minutiae = null;
                    Marshal.FreeHGlobal(pMinList);
                    throw new Exception("GBLFS_NET_ExtractMinutiae: cannot get minlist");
                }

                //////////////////////////////
                // copy into managed minutiae
                // list
                //////////////////////////////
                MINUTIAE LowLevelList = (MINUTIAE)(Marshal.PtrToStructure(minlist, typeof(MINUTIAE)));
                Minutiae = new GBLFS_NET_Minutia[LowLevelList.num];
                if (Minutiae == null)
                {
                    LfsConvdll_NetWrapper.FreeMinutiae(pMinList);
                    Marshal.FreeHGlobal(pMinList);
                    throw new Exception("GBLFS_NET_ExtractMinutiae: cannot alloc Minutiae");
                }
                for (int i = 0; i < LowLevelList.num; i++)
                {
                    IntPtr pLLItem = Marshal.ReadIntPtr(LowLevelList.list, (i * IntPtr.Size));
                    MINUTIA LLItem = (MINUTIA)(Marshal.PtrToStructure(pLLItem, typeof(MINUTIA)));

                    Minutiae[i] = new GBLFS_NET_Minutia(
                        LLItem.x, LLItem.y, LLItem.ex, LLItem.ey, LLItem.direction,
                        LLItem.reliability, LLItem.type, LLItem.appearing,
                        LLItem.feature_id, LLItem.nbrs, LLItem.ridge_counts,
                        LLItem.num_nbrs);

                    if (Minutiae[i] == null)
                    {
                        LfsConvdll_NetWrapper.FreeMinutiae(pMinList);
                        Marshal.FreeHGlobal(pMinList);
                        throw new Exception("GBLFS_NET_ExtractMinutiae: cannot alloc Minutiae[" + i + "]");
                    }
                }

                //////////////////////////////
                // free allocated memory
                //////////////////////////////
                LfsConvdll_NetWrapper.FreeMinutiae(pMinList);
                Marshal.FreeHGlobal(pMinList);
                return GBLFS_NET_Error.Success;
            }
            catch (Exception ex)
            {
                Minutiae = null;
                throw ex;
            }
        }

		/*********************
		** FMC
		*********************/
		public static Int32 GBLFS_NET_ExtractMinutiae(
			Byte []  FMC,
			UInt16 FMCLength, 
			Byte FormatType,
            out GBLFS_NET_Minutia[] Minutiae
        )
        {
            try
            {
                Minutiae = null;

                ////////////////////////////////
                // Prepare Minutiae list pointer
                ////////////////////////////////
                IntPtr pMinList = Marshal.AllocHGlobal(IntPtr.Size);
                if (pMinList == IntPtr.Zero)
                {
                    Minutiae = null;
                    throw new Exception("GBLFS_NET_ExtractMinutiae: cannot alloc pMinList");
                }
                ////////////////////////////////
                // Prepare FMC buffer pointer
                ////////////////////////////////
                IntPtr FmcBuffer = Marshal.AllocHGlobal(FMC.Length);
                if (pMinList == IntPtr.Zero)
                {
                    Minutiae = null;
                    Marshal.FreeHGlobal(pMinList);
                    throw new Exception("GBLFS_NET_ExtractMinutiae: cannot alloc FmcBuffer");
                }
                Marshal.Copy(FMC, 0, FmcBuffer, FMC.Length);

                ////////////////////////////////
                // Call low level function
                ////////////////////////////////
                int RetVal = LfsConvdll_NetWrapper.FMCMinutiaeToLfs(FmcBuffer, FMCLength, FormatType, pMinList);
                // free FMR buffer
                Marshal.FreeHGlobal(FmcBuffer);
                // check low level return value
                if (RetVal != LfsConv_Constants.Success)
                {
                    Marshal.FreeHGlobal(pMinList);
                    Minutiae = null;
                    return RetVal;
                }

                ////////////////////////////////
                // Now get minutiae list pointer
                ////////////////////////////////
                IntPtr minlist = Marshal.ReadIntPtr(pMinList);
                if (minlist == IntPtr.Zero)
                {
                    Minutiae = null;
                    Marshal.FreeHGlobal(pMinList);
                    throw new Exception("GBLFS_NET_ExtractMinutiae: cannot get minlist");
                }

                //////////////////////////////
                // copy into managed minutiae
                // list
                //////////////////////////////
                MINUTIAE LowLevelList = (MINUTIAE)(Marshal.PtrToStructure(minlist, typeof(MINUTIAE)));
                Minutiae = new GBLFS_NET_Minutia[LowLevelList.num];
                if (Minutiae == null)
                {
                    LfsConvdll_NetWrapper.FreeMinutiae(pMinList);
                    Marshal.FreeHGlobal(pMinList);
                    throw new Exception("GBLFS_NET_ExtractMinutiae: cannot alloc Minutiae");
                }
                for (int i = 0; i < LowLevelList.num; i++)
                {
                    IntPtr pLLItem = Marshal.ReadIntPtr(LowLevelList.list, (i * IntPtr.Size));
                    MINUTIA LLItem = (MINUTIA)(Marshal.PtrToStructure(pLLItem, typeof(MINUTIA)));

                    Minutiae[i] = new GBLFS_NET_Minutia(
                        LLItem.x, LLItem.y, LLItem.ex, LLItem.ey, LLItem.direction,
                        LLItem.reliability, LLItem.type, LLItem.appearing,
                        LLItem.feature_id, LLItem.nbrs, LLItem.ridge_counts,
                        LLItem.num_nbrs);

                    if (Minutiae[i] == null)
                    {
                        LfsConvdll_NetWrapper.FreeMinutiae(pMinList);
                        Marshal.FreeHGlobal(pMinList);
                        throw new Exception("GBLFS_NET_ExtractMinutiae: cannot alloc Minutiae[" + i + "]");
                    }
                }

                //////////////////////////////
                // free allocated memory
                //////////////////////////////
                LfsConvdll_NetWrapper.FreeMinutiae(pMinList);
                Marshal.FreeHGlobal(pMinList);
                return GBLFS_NET_Error.Success;
            }
            catch (Exception ex)
            {
                Minutiae = null;
                throw ex;
            }
        }

		///////////////////////////////////////////
		// MINUTIAE MATCHING
		///////////////////////////////////////////
		public static Int32 GBLFS_NET_Bozorth(
			Int32 MaxNumberOfMinutiaeToConsider,
            GBLFS_NET_Minutia[] Probe,
            GBLFS_NET_Minutia[] Gallery,
			out Int32 Score
        )
        {
            try
            {
                if (Probe == null || Gallery == null)
                {
                    Score = 0;
                    throw new Exception("GBLFS_NET_Bozorth: probe and gallery cannot be null");
                }

                /////////////////////////////
                // build probe minutiae list
                /////////////////////////////
                MINUTIAE probeList = new MINUTIAE();
                probeList.list = Marshal.AllocHGlobal(Probe.Length * IntPtr.Size);
                if (probeList.list == IntPtr.Zero)
                {
                    // throw exception
                    Score = 0;
                    throw new Exception("GBLFS_NET_Bozorth: not enough memory for probeList.list");
                }
                // build list
                for (int i = 0; i < Probe.Length; i++)
                {
                    // create list item
                    MINUTIA Item = Probe[i].ConvertToLowLevelMinutia();
                    // get pointer to item
                    IntPtr pItem = Marshal.AllocHGlobal(Marshal.SizeOf(Item));
                    Marshal.StructureToPtr(Item, pItem, false);
                    if (pItem == IntPtr.Zero)
                    {
                        // free memory
                        for (int j = 0; j < i; j++)
                        {
                            IntPtr Dummy = Marshal.ReadIntPtr(probeList.list, j * IntPtr.Size);
                            Marshal.FreeHGlobal(Dummy);
                        }
                        Marshal.FreeHGlobal(probeList.list);
                        // throw exception
                        Exception ex = new Exception("GBLFS_NET_Bozorth: not enough memory for probe pItem[" + i + "]");
                        throw (ex);
                    }
                    // write ptr in list
                    Marshal.WriteIntPtr(probeList.list, i * IntPtr.Size, pItem);
                }
                probeList.num = Probe.Length;

                /////////////////////////////
                // build gallery minutiae list
                /////////////////////////////
                MINUTIAE galleryList = new MINUTIAE();
                galleryList.list = Marshal.AllocHGlobal(Gallery.Length * IntPtr.Size);
                if (galleryList.list == IntPtr.Zero)
                {
                    // throw exception
                    Score = 0;
                    for (int i = 0; i < probeList.num; i++)
                    {
                        IntPtr Dummy = Marshal.ReadIntPtr(probeList.list, i * IntPtr.Size);
                        Marshal.FreeHGlobal(Dummy);
                    }
                    Marshal.FreeHGlobal(probeList.list);
                    throw new Exception("GBLFS_NET_Bozorth: not enough memory for galleryList.list");
                }
                // build list
                for (int i = 0; i < Gallery.Length; i++)
                {
                    // create list item
                    MINUTIA Item = Gallery[i].ConvertToLowLevelMinutia();
                    // get pointer to item
                    IntPtr pItem = Marshal.AllocHGlobal(Marshal.SizeOf(Item));
                    if (pItem == IntPtr.Zero)
                    {
                        // free memory
                        for (int j = 0; j < i; j++)
                        {
                            IntPtr Dummy = Marshal.ReadIntPtr(galleryList.list, j * IntPtr.Size);
                            Marshal.FreeHGlobal(Dummy);
                        }
                        Marshal.FreeHGlobal(galleryList.list);
                        // throw exception
                        Exception ex = new Exception("GBLFS_NET_Bozorth: not enough memory for gallery pItem[" + i + "]");
                        throw (ex);
                    }
                    Marshal.StructureToPtr(Item, pItem, false);
                    // write ptr in list
                    Marshal.WriteIntPtr(galleryList.list, i * IntPtr.Size, pItem);
                }
                galleryList.num = Gallery.Length;

                //////////////////////////////////////
                // call low level function
                //////////////////////////////////////
                int RetVal = Bozorthdll_NetWrapper.boz_direct_call(MaxNumberOfMinutiaeToConsider, ref probeList, ref galleryList);

                //////////////////////
                // Free memory
                //////////////////////
                for (int i = 0; i < galleryList.num; i++)
                {
                    IntPtr Dummy = Marshal.ReadIntPtr(galleryList.list, i * IntPtr.Size);
                    Marshal.FreeHGlobal(Dummy);
                }
                Marshal.FreeHGlobal(galleryList.list);
                for (int i = 0; i < probeList.num; i++)
                {
                    IntPtr Dummy = Marshal.ReadIntPtr(probeList.list, i * IntPtr.Size);
                    Marshal.FreeHGlobal(Dummy);
                }
                Marshal.FreeHGlobal(probeList.list);

                if (RetVal > 0)
                {
                    Score = RetVal;
                    return GBLFS_NET_Error.Success;
                }
                Score = 0;
                return GBLFS_NET_Error.Failure;
            }
            catch (Exception ex)
            {
                Score = 0;
                throw ex;
            }
        }
	};
}
