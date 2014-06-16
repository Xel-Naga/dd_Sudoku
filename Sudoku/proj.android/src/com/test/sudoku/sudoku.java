/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.test.sudoku;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxEditText;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.cocos2dx.lib.Cocos2dxRenderer;

import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.pm.ConfigurationInfo;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.LinearLayout;

import cn.waps.AdInfo;
import cn.waps.AppConnect;
import cn.waps.UpdatePointsNotifier;
import cn.waps.extend.AppDetail;
import cn.waps.extend.AppWall;
import cn.waps.extend.QuitPopAd;

import com.wanpu.pay.PayConnect;
import com.wanpu.pay.PayResultListener;

public class sudoku extends Cocos2dxActivity {
	private static Context mContext;
	private static Handler handler;
	private LinearLayout miniLayout;
	private Cocos2dxGLSurfaceView mGLView;
	
	// 应用或游戏商自定义的支付订单(每条支付订单数据不可相同)
	String orderId = "9999999999";
	// 用户标识
	String userId = "";
	// 支付商品名称
	String goodsName = "测试商品";
	// 支付金额
	float price = 0.0f;
	// 支付时间
	String time = "";
	// 支付描述
	String goodsDesc = "";
	// 应用或游戏商服务器端回调接口（无服务器可不填写）
	String notifyUrl = "";
	
	protected void onDestroy(){
		AppConnect.getInstance(this).close();
	}
	
	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		mContext = this;
		
		//支付初始化
		PayConnect.getInstance("c6a9b1d560e554e84202c0b538a68e05", "91", mContext);
		
		userId = PayConnect.getInstance(mContext).getDeviceId(mContext);

		handler = new Handler() {
			@Override
			public void handleMessage(Message msg) {
				// TODO Auto-generated method stub
				switch (msg.what) {
				case 0:
					// 显示推荐列表（综合）
					AppConnect.getInstance(mContext).showOffers(mContext);
					break;
				case 1:
					// 显示插屏广告
					// 判断插屏广告是否已初始化完成，用于确定是否能成功调用插屏广告
					boolean hasPopAd = AppConnect.getInstance(mContext)
							.hasPopAd(mContext);
					System.out.println("------------------" + hasPopAd);
					if (hasPopAd) {
						AppConnect.getInstance(mContext).showPopAd(mContext);
						System.out.println("------执行了showPopAd--------");
					}
					break;
				case 2:
					// 显示推荐列表（软件）
					AppConnect.getInstance(mContext).showAppOffers(mContext);
					break;
				case 3:
					// 显示推荐列表（游戏）
					AppConnect.getInstance(mContext).showGameOffers(mContext);
					break;
				case 4:
					// 获取全部自定义广告数据
					Intent appWallIntent = new Intent(mContext, AppWall.class);
					mContext.startActivity(appWallIntent);
					break;
				case 5:
					// 获取一条自定义广告数据
					AdInfo adInfo = AppConnect.getInstance(mContext).getAdInfo();
					AppDetail.getInstanct().showAdDetail(mContext, adInfo);
					break;
				case 8:
					// 显示自家应用列表
					AppConnect.getInstance(mContext).showMore(mContext);
					break;
				case 9:
					// 根据指定的应用app_id展示其详情
					AppConnect.getInstance(mContext).showMore(mContext,
							"c6a9b1d560e554e84202c0b538a68e05");
					break;
				case 10:
					// 调用功能广告接口（使用浏览器接口）
					String uriStr = "http://www.baidu.com";
					AppConnect.getInstance(mContext).showBrowser(mContext,
							uriStr);
					break;
				case 11:
					// 用户反馈
					AppConnect.getInstance(mContext).showFeedback(mContext);
					break;
				case 12:
					// 退屏广告
					QuitPopAd.getInstance().show(mContext);
					break;
				case 13:
					AppConnect.getInstance(mContext).showMiniAd(mContext, miniLayout, 10);
					break;
				case 14:
					// 游戏商自定义支付订单号，保证该订单号的唯一性，建议在执行支付操作时才进行该订单号的生成
			        orderId = System.currentTimeMillis() + "";
			        price = 0.0f;
					
					PayConnect.getInstance(mContext).pay(mContext, 
																	orderId, 
																	userId, 
																	price, 
																	goodsName, 
																	goodsDesc, 
																	notifyUrl,
																	new MyPayResultListener());
				}
			}
		};

		if (detectOpenGLES20()) {
			
			AppConnect.getInstance("c6a9b1d560e554e84202c0b538a68e05",
					"91", mContext);
			
			// FrameLayout
			ViewGroup.LayoutParams framelayout_params = new ViewGroup.LayoutParams(
					ViewGroup.LayoutParams.FILL_PARENT,
					ViewGroup.LayoutParams.FILL_PARENT);
			FrameLayout framelayout = new FrameLayout(this);
			framelayout.setLayoutParams(framelayout_params);

			// Cocos2dxEditText layout
			ViewGroup.LayoutParams edittext_layout_params = new ViewGroup.LayoutParams(
					ViewGroup.LayoutParams.FILL_PARENT,
					ViewGroup.LayoutParams.WRAP_CONTENT);
			Cocos2dxEditText edittext = new Cocos2dxEditText(this);
			edittext.setLayoutParams(edittext_layout_params);

			// ...add to FrameLayout
			framelayout.addView(edittext);

			// Cocos2dxGLSurfaceView
			mGLView = new Cocos2dxGLSurfaceView(this);

			// ...add to FrameLayout
			framelayout.addView(mGLView);

			mGLView.setEGLContextClientVersion(2);
			mGLView.setCocos2dxRenderer(new Cocos2dxRenderer());
			//mGLView.setTextField(edittext);

			// Set framelayout as the content view
			setContentView(framelayout);
			// 迷你广告
			LinearLayout adMiniLayout = new LinearLayout(this);
			adMiniLayout.setOrientation(LinearLayout.VERTICAL);
			FrameLayout.LayoutParams lp_mini = new FrameLayout.LayoutParams(
					FrameLayout.LayoutParams.FILL_PARENT,
					FrameLayout.LayoutParams.WRAP_CONTENT);
			// 设置adMiniLayout的悬浮位置,具体的位置开发者根据需要设置
			lp_mini.gravity = Gravity.CENTER_VERTICAL | Gravity.CENTER_HORIZONTAL;
			this.addContentView(adMiniLayout, lp_mini);

			miniLayout = new LinearLayout(this);
			miniLayout.setGravity(Gravity.CENTER_HORIZONTAL);
			adMiniLayout.addView(miniLayout);
				
		} else {
			Log.d("activity", "don't support gles2.0");
			finish();
		}
	}
	
	/**
	 * 自定义Listener实现PaySuccessListener，用于监听支付成功
	 * @author Administrator
	 *
	 */
	private class MyPayResultListener implements PayResultListener{

		@Override
		public void onPayFinish(Context payViewContext, String orderId, int resultCode, String resultString, int payType,
				float amount, String goodsName) {
			// 可根据resultCode自行判断
			if(resultCode == 0){
				//Toast.makeText(getApplicationContext(), resultString + "：" + amount + "元", Toast.LENGTH_LONG).show();
				// 支付成功时关闭当前支付界面
				PayConnect.getInstance(sudoku.this).closePayView(payViewContext);	
				
				// TODO 在客户端处理支付成功的操作
				
				// 未指定notifyUrl的情况下，交易成功后，必须发送回执
				PayConnect.getInstance(sudoku.this).confirm(orderId, payType);
			}else{
				//Toast.makeText(getApplicationContext(), resultString, Toast.LENGTH_LONG).show();
			}
		}
	}
	
	// 向handler发送要展示Banner的消息
	public static void showAdStatic(int adTag) {
		Message msg = handler.obtainMessage();
		msg.what = adTag; // 私有静态的整型变量，开发者请自行定义值
		handler.sendMessage(msg);
	}

	@Override
	public void onWindowFocusChanged(boolean hasFocus) {
		// TODO Auto-generated method stub
		//setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		super.onWindowFocusChanged(hasFocus);
	}

	@Override
	protected void onPause() {
		super.onPause();
	}

	@Override
	protected void onResume() {
		super.onResume();
	}

	private boolean detectOpenGLES20() {
		ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
		ConfigurationInfo info = am.getDeviceConfigurationInfo();
		return (info.reqGlEsVersion >= 0x20000);
	}

    static {
         System.loadLibrary("game");
    }
}
