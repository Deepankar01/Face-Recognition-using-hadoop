package live.abhinav.facesearch.app;
import android.app.Activity;
import android.app.Fragment;
import android.app.ProgressDialog;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Typeface;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;
import java.io.FileOutputStream;

public class ImageFragment extends Fragment {
    private static final int REQUEST_CODE = 100;
    ProgressDialog dialog = null;
    ImageView imageView;
    int serverResponseCode = 0;
    RelativeLayout view;
    TextView faceSearch;
    Communicator comm;

    public ImageFragment() {
        // Required empty public constructor
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        view = (RelativeLayout) inflater.inflate(R.layout.fragment_image, container, false);

        Typeface robotoThin = Typeface.createFromAsset(getActivity().getAssets(), "fonts/roboto_thin.ttf");
        faceSearch = (TextView) view.findViewById(R.id.faceSearch);

        faceSearch.setTypeface(robotoThin);
        return view;
    }

    public void clickedButton() {
        Log.d("Lifecycle", "clickedButton() called");
        imageView = (ImageView) view.findViewById(R.id.clickedPic);
        if (imageView != null)
            callCamera();
//        else
//            Toast.makeText(getActivity(), "Null", Toast.LENGTH_LONG).show();
    }

    //work of camera begins
    public void callCamera() {
        Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        startActivityForResult(intent, REQUEST_CODE);
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == REQUEST_CODE) {
            if (resultCode == MainActivity.RESULT_OK) {
                Bitmap bp = (Bitmap) data.getExtras().get("data");
                imageView.setImageBitmap(bp);
                comm.respond(bp);
                // Image captured and saved to fileUri specified in the Intent
                data.setData(Uri.parse("/storage/sdcard/saved_images/"));
                SaveImage(bp);
                faceSearch.setVisibility(View.GONE);

                Log.d("Lifecycle", "Data saved to " + data.getData());
            } else if (resultCode == MainActivity.RESULT_CANCELED) {
                Toast.makeText(getActivity(), "User cancelled the operation", Toast.LENGTH_LONG).show();
            } else {
                // Image capture failed, advise user
                Toast.makeText(getActivity(), "Image Capture Failed", Toast.LENGTH_LONG).show();
            }
        }
    }

    private void SaveImage(Bitmap finalBitmap) {
        File file = getActivity().getFilesDir();
        if (file.exists()) file.delete();
        try {
            FileOutputStream out = getActivity().openFileOutput("image.jpg", MainActivity.MODE_PRIVATE);
            finalBitmap.compress(Bitmap.CompressFormat.JPEG, 90, out);
            out.flush();
            out.close();
            Log.d("Lifecycle", "Image saved to " + file.getPath());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
//Camera works ends
    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        comm = (Communicator) activity;
    }

    @Override
    public void onDetach() {
        super.onDetach();
    }
}