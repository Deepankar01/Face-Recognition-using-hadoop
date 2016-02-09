package live.abhinav.facesearch.app;

import android.app.Activity;
import android.app.Fragment;
import android.graphics.Bitmap;
import android.graphics.Typeface;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;
import org.json.JSONException;
import org.json.JSONObject;

public class ResultFragment extends Fragment {
    RelativeLayout view;
//    TextView textView;

    public ResultFragment() {
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
        view = (RelativeLayout) inflater.inflate(R.layout.fragment_result, container, false);
        Typeface roboto_light = Typeface.createFromAsset(getActivity().getAssets(), "fonts/roboto-light.ttf");
//        textView = (TextView) view.findViewById(R.id.quality);
        TextView name = (TextView) view.findViewById(R.id.name);
        name.setTypeface(roboto_light);
        return view;
    }

    // TODO: Rename method, update argument and hook method into UI event
    public void showResults(String result, Bitmap bp) throws JSONException {
        ImageView pic = (ImageView) view.findViewById(R.id.picture);
        pic.setImageBitmap(bp);
        JSONObject resultJson = new JSONObject(result);
        String nameR = resultJson.getString("name");
        String qualityR = resultJson.getString("quality");
        String statusR = resultJson.getString("status");

        TextView name = (TextView) view.findViewById(R.id.name);
        TextView quality = (TextView) view.findViewById(R.id.quality);
        TextView aka= (TextView) view.findViewById(R.id.aka);

        if(nameR.equals(""))
        {
            nameR="No Match Found";
            qualityR="";
            aka.setVisibility(View.GONE);
        }
        else
        {
            aka.setVisibility(View.VISIBLE);
        }
        name.setText(nameR);
        quality.setText(qualityR);


//        quality.setText(result);
    }

    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
    }

    @Override
    public void onDetach() {
        super.onDetach();
    }
}