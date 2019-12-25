package com.greenbit.MultiscanJNIGuiJavaAndroid;


import android.content.Context;

import com.itextpdf.text.BadElementException;
import com.itextpdf.text.Document;
import com.itextpdf.text.DocumentException;
import com.itextpdf.text.Image;
import com.itextpdf.text.Rectangle;
import com.itextpdf.text.pdf.PdfContentByte;
import com.itextpdf.text.pdf.PdfGState;
import com.itextpdf.text.pdf.PdfPageEventHelper;
import com.itextpdf.text.pdf.PdfReader;
import com.itextpdf.text.pdf.PdfStamper;
import com.itextpdf.text.pdf.PdfWriter;

import java.io.FileOutputStream;
import java.io.IOException;


public class PDFEventListener extends PdfPageEventHelper {
    Context context;
    byte[] byteArray;

    public PDFEventListener(Context context, byte[] byteArray) {
        this.byteArray = byteArray;
        this.context = context;
    }

    @Override
    public void onEndPage(PdfWriter writer, Document document) {
//        PdfContentByte canvas = writer.getDirectContentUnder();
//        Phrase watermark = new Phrase("JCG", new Font(FontFamily.TIMES_ROMAN, 190, Font.NORMAL, BaseColor.LIGHT_GRAY));
//        ColumnText.showTextAligned(canvas, Element.ALIGN_CENTER, watermark, 337, 500, 45);
// read existing pdf to add watermark
        PdfReader reader = null;
        try {
            reader = new PdfReader(Common.getAppPath(context) + "test_pdf.pdf");
        } catch (IOException e) {
            e.printStackTrace();
        }
        PdfStamper stamper = null;
        try {
            stamper = new PdfStamper(reader, new FileOutputStream(Common.getAppPath(context) + "test_pdf.pdf"));
        } catch (DocumentException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        // image watermark
        Image img = null;
        try {
            img = Image.getInstance(byteArray);
        } catch (BadElementException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
//            Image img = Image.getInstance(getResources("/logo.png"));
        float w = img.getScaledWidth();
        float h = img.getScaledHeight();


        // properties
        PdfContentByte over;
        Rectangle pagesize;
        float x, y;
        // loop over every page
        int n = reader.getNumberOfPages();
        for (int i = 1; i <= n; i++) {

            // get page size and position
            pagesize = reader.getPageSizeWithRotation(i);
            x = (pagesize.getLeft() + pagesize.getRight()) / 2;
            y = (pagesize.getTop() + pagesize.getBottom()) / 2;
            over = stamper.getOverContent(i);
            over.saveState();

            // set transparency
            PdfGState state = new PdfGState();
            state.setFillOpacity(0.2f);
            over.setGState(state);

            // add watermark image

            try {
                over.addImage(img, w, 0, 0, h, x - (w / 2), y - (h / 2));
            } catch (DocumentException e) {
                e.printStackTrace();
            }


            over.restoreState();
        }
        try {
            stamper.close();
            reader.close();
        } catch (DocumentException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }


    }
}